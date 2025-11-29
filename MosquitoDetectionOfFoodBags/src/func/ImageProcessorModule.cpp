#include "ImageProcessorModule.hpp"

#include "MDOFoodBags.h"
#include "Modules.hpp"
#include "rqw_HalconUtilty.hpp"
#include "Utilty.hpp"
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QColor>
ImageProcessor::ImageProcessor(QQueue<MatInfo>& queue, QMutex& mutex, QWaitCondition& condition, int workIndex, QObject* parent)
	: QThread(parent), _queue(queue), _mutex(mutex), _condition(condition), _workIndex(workIndex)
{

}

void ImageProcessor::run()
{
	while (!QThread::currentThread()->isInterruptionRequested()) {
		MatInfo frame;
		{
			QMutexLocker locker(&_mutex);
			if (_queue.isEmpty()) {
				_condition.wait(&_mutex);
				if (QThread::currentThread()->isInterruptionRequested()) {
					break;
				}
			}
			if (!_queue.isEmpty()) {
				frame = _queue.dequeue();
			}
			else {
				continue; // 如果队列仍为空，跳过本次循环
			}
		}

		// 检查 frame 是否有效
		if (frame.image.empty()) {
			continue; // 跳过空帧
		}

		// 获取当前时间点
		auto now = std::chrono::system_clock::now();
		// 转换为time_t格式
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
		// 转换为本地时间
		std::tm* local_time = std::localtime(&now_time);

		auto currentRunningState = Modules::getInstance().runtimeInfoModule.runningState.load();
		switch (currentRunningState)
		{
		case RunningState::Debug:
			run_debug(frame);
			break;
		case RunningState::OpenRemoveFunc:
			run_OpenRemoveFunc(frame);
			break;
		default:
			break;
		}
	}
}

void ImageProcessor::run_debug(MatInfo& frame)
{
	//auto& imgPro = *_imgProcess;
	//imgPro(frame.image);
	//auto maskImg = imgPro.getMaskImg(frame.image);
	//auto defectResult = imgPro.getDefectResultInfo();

	auto image = rw::rqw::cvMatToQImage(frame.image);

	emit imageNGReady(QPixmap::fromImage(image), frame.index, false);

	rw::rqw::ImageInfo imageInfo(rw::rqw::cvMatToQImage(frame.image));
	save_image(imageInfo, image);
}

void ImageProcessor::run_OpenRemoveFunc(MatInfo& frame)
{
	auto image = rw::rqw::cvMatToQImage(frame.image);

	//显示图片测试
	//QString imagePath = "C:/Users/zfkj4090/Desktop/temp/image.jpg";
	//QImage image(imagePath);
	//进行图像处理
	//4个qvector用来存坏的数据的位置，还有面积
	QVector< double> R1, C1, R2, C2, Areas;

	halconPRocess(frame.image, R1, C1, R2, C2, Areas);
	//TODO:这两个最小面积和最小总面积设置成系统参数

	double minArea = 0;
	double allMinArea = 0;

	//这个函数可以判断是不是坏的，并且在图像上画出矩形，还要绘制左右限位
	_isbad = checkDefectAndDrawOnImage(image, R1, C1, R2, C2, Areas, minArea, allMinArea);



	run_OpenRemoveFunc_emitErrorInfo(_isbad);

	emit imageNGReady(QPixmap::fromImage(image), frame.index, _isbad);

	rw::rqw::ImageInfo imageInfo(image);
	save_image(imageInfo, image);
}

void ImageProcessor::halconPRocess(cv::Mat image, QVector< double>& R1, QVector< double>& C1, QVector< double>& R2, QVector< double>& C2, QVector< double>& Area)
{
	//模板图片
	HalconCpp::HObject  ho_ImageModel, ho_Image;
	HalconCpp::HObject   ho_ImageModelMean;
	HalconCpp::HObject  ho_ImageMean, ho_ImageSub, ho_Rectangle, ho_ImageReduced;
	HalconCpp::HObject  ho_Regions, ho_RegionTrans;


	HalconCpp::HTuple  hv_Width, hv_Height, hv_marskx, hv_marsky;
	HalconCpp::HTuple  hv_zuoxianwei, hv_youxianwei, hv_huidumin, hv_Area, hv_shangxiasuojin;
	HalconCpp::HTuple  hv_Row, hv_Column, hv_shapetransRow1, hv_shapetransColumn1;
	HalconCpp::HTuple  hv_shapetransRow2, hv_shapetransColumn2, hv_Index;
	double xiangsudangliang = Modules::getInstance().configManagerModule.setConfig.xiangSuDangLiang1;

	//需要认为设置的参数现在硬编码了
	hv_marskx = 9;
	hv_marsky = 9;
	hv_zuoxianwei = Modules::getInstance().configManagerModule.setConfig.zuoXianWei1;
	hv_youxianwei = Modules::getInstance().configManagerModule.setConfig.youXianWei1;
	hv_shangxiasuojin = Modules::getInstance().configManagerModule.setConfig.shangXianWei1;


	hv_huidumin = 140;



	// ho_ImageModel = MDOFoodBags::getModelImage();
	 auto modelImagePtr = MDOFoodBags::getModelImage();
	 if (modelImagePtr && modelImagePtr->IsInitialized()) {
		 ho_ImageModel = *modelImagePtr;
	 }
	/*HalconCpp::ReadImage(&ho_ImageModel, "C:/Users/zfkj4090/Desktop/temp/model.jpg");
	HalconCpp::Rgb1ToGray(ho_ImageModel, &ho_ImageModel);
	MDOFoodBags::setModelImage(ho_ImageModel);*/
	//modelimage = MDOFoodBags::getModelImage();






	//当前图片
	ho_Image = rw::rqw::CvMatToHImage(image);
	//HalconCpp::ReadImage(&ho_Image, "C:/Users/zfkj4090/Desktop/temp/image.jpg");
	HalconCpp::Rgb1ToGray(ho_Image, &ho_Image);

	//获取图片尺寸

	GetImageSize(ho_Image, &hv_Width, &hv_Height);




	if (!modelImagePtr || !modelImagePtr->IsInitialized()) {
		return;
	}
	MeanImage(ho_ImageModel, &ho_ImageModelMean, hv_marskx, hv_marsky);


	MeanImage(ho_Image, &ho_ImageMean, hv_marskx, hv_marsky);



	SubImage(ho_ImageModelMean, ho_ImageMean, &ho_ImageSub, 1, 128);
	if (hv_Height < hv_shangxiasuojin)
	{

		hv_shangxiasuojin = 0;
	}
	GenRectangle1(&ho_Rectangle, 20, hv_zuoxianwei, hv_Height - 20, hv_youxianwei);

	ReduceDomain(ho_ImageSub, ho_Rectangle, &ho_ImageReduced);

	Threshold(ho_ImageReduced, &ho_Regions, hv_huidumin, 255);

	AreaCenter(ho_Regions, &hv_Area, &hv_Row, &hv_Column);

	ShapeTrans(ho_Regions, &ho_RegionTrans, "rectangle1");
	SmallestRectangle1(ho_RegionTrans, &hv_shapetransRow1, &hv_shapetransColumn1, &hv_shapetransRow2,
		&hv_shapetransColumn2);


	if (0 != (int((hv_shapetransRow1.TupleLength()) > 0)))
	{
		{
			HalconCpp::HTuple end_val39 = (hv_shapetransRow1.TupleLength()) - 1;
			HalconCpp::HTuple step_val39 = 1;
			for (hv_Index = 0; hv_Index.Continue(end_val39, step_val39); hv_Index += step_val39)
			{
				if (hv_shapetransRow1[hv_Index] > 0)
				{
					R1.append(hv_shapetransRow1[hv_Index]);
					C1.append(hv_shapetransColumn1[hv_Index]);
					R2.append(hv_shapetransRow2[hv_Index]);
					C2.append(hv_shapetransColumn2[hv_Index]);
					Area.append(hv_Area[hv_Index] * xiangsudangliang * xiangsudangliang);
				}
			}
		}


		//有坏的


	}





}
void ImageProcessor::drawSingleRectangleOnImage(QImage& image, double R1, double C1, double R2, double C2, double area, const QColor& color, int penWidth)
{
	// 检查图像是否有效
	if (image.isNull())
	{
		return;
	}

	// 创建 QPainter 对象用于在图像上绘制
	QPainter painter(&image);

	// 设置抗锯齿,使绘制的矩形边缘更平滑
	painter.setRenderHint(QPainter::Antialiasing, true);

	// 设置画笔颜色和宽度
	QPen pen(color);
	pen.setWidth(penWidth);
	painter.setPen(pen);

	// Halcon 坐标系: R 表示 Row (行,对应 y 坐标), C 表示 Column (列,对应 x 坐标)
	// R1, C1 是矩形左上角坐标
	// R2, C2 是矩形右下角坐标
	int x1 = static_cast<int>(C1);
	int y1 = static_cast<int>(R1);
	int x2 = static_cast<int>(C2);
	int y2 = static_cast<int>(R2);

	// 计算矩形的宽度和高度
	int width = x2 - x1;
	int height = y2 - y1;

	// 绘制矩形框
	painter.drawRect(x1, y1, width, height);

	// 绘制面积文本
	QFont font = painter.font();
	font.setPointSize(30);  // 字体大小
	font.setBold(true);     // 加粗
	painter.setFont(font);

	// 准备文本内容
	QString areaText = QString("%1 mm²").arg(area, 0, 'f', 2);

	// 计算文本位置(矩形左上角上方)
	QFontMetrics metrics(font);
	QRect textRect = metrics.boundingRect(areaText);

	// 将文本放置在矩形上方,居中对齐
	int textX = x1 + (width - textRect.width()) / 2;
	int textY = y1 - 10;  // 矩形上方10像素处

	// 如果文本会超出图像顶部,则放在矩形内部顶部
	if (textY - textRect.height() < 0)
	{
		textY = y1 + textRect.height() + 10;
	}

	textRect.moveTo(textX, textY - textRect.height());
	textRect.adjust(-5, -5, 5, 5);  // 扩展矩形边距

	// 绘制半透明背景
	painter.fillRect(textRect, QColor(0, 0, 0, 180));

	// 绘制文本(白色)
	painter.setPen(Qt::white);
	painter.drawText(textRect, Qt::AlignCenter, areaText);

	// 结束绘制
	painter.end();
}
void ImageProcessor::drawRectanglesOnImage(QImage& image, const QVector<double>& R1, const QVector<double>& C1, const QVector<double>& R2, const QVector<double>& C2, const QVector<double>& Areas, const QColor& color, int penWidth)
{
	// 检查图像是否有效
	if (image.isNull())
	{
		return;
	}

	// 检查所有向量大小是否一致
	if (R1.size() != C1.size() || R1.size() != R2.size() || R1.size() != C2.size())
	{
		return;
	}

	// 如果没有矩形需要绘制,直接返回
	if (R1.isEmpty())
	{
		return;
	}

	// 创建 QPainter 对象用于在图像上绘制
	QPainter painter(&image);

	// 设置抗锯齿,使绘制的矩形边缘更平滑
	painter.setRenderHint(QPainter::Antialiasing, true);

	// 设置画笔颜色和宽度
	QPen pen(color);
	pen.setWidth(penWidth);
	painter.setPen(pen);

	// 计算总面积
	double totalArea = 0.0;
	for (const auto& area : Areas)
	{
		totalArea += area;
	}

	// 在图片左上角绘制总面积文本
	// 设置字体
	QFont font = painter.font();
	font.setPointSize(40);  // 字体大小
	font.setBold(true);     // 加粗
	painter.setFont(font);

	// 准备文本内容
	QString areaText = QString("总面积: %1 mm²").arg(totalArea, 0, 'f', 2);

	// 设置文本背景
	QFontMetrics metrics(font);
	QRect textRect = metrics.boundingRect(areaText);
	textRect.moveTo(10, 10);  // 文本位置(左上角,留10像素边距)
	textRect.adjust(-5, -5, 5, 5);  // 扩展矩形边距

	// 绘制半透明背景
	painter.fillRect(textRect, QColor(0, 0, 0, 180));

	// 绘制文本(白色)
	painter.setPen(Qt::white);
	painter.drawText(textRect, Qt::AlignCenter, areaText);

	// 恢复画笔颜色用于绘制矩形
	painter.setPen(pen);

	// 遍历所有矩形并绘制
	for (int i = 0; i < R1.size(); ++i)
	{
		// Halcon 坐标系: R 表示 Row (行,对应 y 坐标), C 表示 Column (列,对应 x 坐标)
		// R1, C1 是矩形左上角坐标
		// R2, C2 是矩形右下角坐标
		int x1 = static_cast<int>(C1[i]);
		int y1 = static_cast<int>(R1[i]);
		int x2 = static_cast<int>(C2[i]);
		int y2 = static_cast<int>(R2[i]);

		// 计算矩形的宽度和高度
		int width = x2 - x1;
		int height = y2 - y1;

		// 绘制矩形框
		painter.drawRect(x1, y1, width, height);
	}

	// 结束绘制
	painter.end();
}


bool ImageProcessor::checkDefectAndDrawOnImage(
	QImage& image,
	const QVector<double>& R1,
	const QVector<double>& C1,
	const QVector<double>& R2,
	const QVector<double>& C2,
	const QVector<double>& Areas,
	double minArea,
	double allMinArea)
{
	bool isbad = false;

	//绘制左右限位
	double zuoxianwei = Modules::getInstance().configManagerModule.setConfig.zuoXianWei1;
	double youxianwei = Modules::getInstance().configManagerModule.setConfig.youXianWei1;
	// 绘制限位线
	drawLimitLines(image, zuoxianwei, youxianwei, Qt::yellow, 3);


	// 说明有瑕疵
	if (Areas.size() > 0)
	{
		// 计算总面积
		double totalArea = 0.0;
		for (const auto& area : Areas)
		{
			totalArea += area;
		}

		// 判断是否为坏袋子:
		// 1. 总面积超过最小总面积阈值
		// 2. 或者存在单个面积超过最小面积阈值

		// 检查是否有单个面积超过阈值并绘制
		for (int i = 0; i < Areas.size(); i++)
		{
			if (Areas[i] >= minArea)
			{
				drawSingleRectangleOnImage(image, R1[i], C1[i], R2[i], C2[i], Areas[i], Qt::red, 3);
				isbad = true;
			}
			else
			{
				drawSingleRectangleOnImage(image, R1[i], C1[i], R2[i], C2[i], Areas[i], Qt::green, 3);
			}
		}

		// 检查总面积是否超过阈值
		if (totalArea >= allMinArea)
		{
			isbad = true;
			// 在图像上绘制检测到的矩形区域
			// 红色表示缺陷区域，线宽为3像素
			drawRectanglesOnImage(image, R1, C1, R2, C2, Areas, Qt::red, 3);
		}
	}

	return isbad;
}





void ImageProcessor::run_OpenRemoveFunc_emitErrorInfo(bool isbad) const
{
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	auto& priorityQueue1 = Modules::getInstance().eliminateModule.productPriorityQueue1;
	if (isbad)
	{
		++statisticalInfo.wasteCount;
	}

	if (imageProcessingModuleIndex == 1 || imageProcessingModuleIndex == 2)
	{
		++statisticalInfo.produceCount;
	}

	/*if (isbad)
	{
		for (const auto& item : DefectBox)
		{
			if (1 == imageProcessingModuleIndex)
			{
				priorityQueue1->push(item);
			}
		}
	}*/
}

void ImageProcessor::save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image)
{
	auto& isTakePictures = Modules::getInstance().runtimeInfoModule.isTakePictures;

	if (!isTakePictures)
	{
		return;
	}

	if ((imageProcessingModuleIndex == 1))
	{
		save_image_work(imageInfo, image);
	}
}

void ImageProcessor::save_image_work(rw::rqw::ImageInfo& imageInfo, const QImage& image)
{
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
	if (_isbad) {

		if (1 == imageProcessingModuleIndex)
		{
			imageInfo.classify = "NG1";
			imageSaveEngine->pushImage(imageInfo);
		}
	}
}

void ImageProcessor::buildSegModelEngine(const QString& enginePath)
{

}

void ImageProcessingModule::BuildModule()
{
	for (int i = 0; i < _numConsumers; ++i) {
		static size_t workIndexCount = 0;
		ImageProcessor* processor = new ImageProcessor(_queue, _mutex, _condition, workIndexCount, this);
		workIndexCount++;
		processor->imageProcessingModuleIndex = index;
		//processor->buildSegModelEngine(modelEnginePath);
		connect(processor, &ImageProcessor::imageNGReady, this, &ImageProcessingModule::imageNGReady, Qt::QueuedConnection);
		_processors.push_back(processor);
		processor->start();
	}
}

ImageProcessingModule::ImageProcessingModule(int numConsumers, QObject* parent)
	: QObject(parent), _numConsumers(numConsumers)
{

}

ImageProcessingModule::~ImageProcessingModule()
{
	// 通知所有线程退出
	for (auto processor : _processors) {
		processor->requestInterruption();
	}

	// 唤醒所有等待的线程
	{
		QMutexLocker locker(&_mutex);
		_condition.wakeAll();
	}

	// 等待所有线程退出
	for (auto processor : _processors) {
		if (processor->isRunning()) {
			processor->wait(1000); // 使用超时机制，等待1秒
		}
		delete processor;
	}
}

void ImageProcessingModule::onFrameCaptured(rw::rqw::MatInfo matInfo, size_t index, float loc)
{
	// 手动读取本地图片
	//std::string imagePath = R"(C:\Users\rw\Desktop\temp\shijin_3.png)"; // 替换为你的图片路径
	//cv::Mat frame1 = cv::imread(imagePath, cv::IMREAD_COLOR);
	//matInfo.mat= frame1.clone();

	if (matInfo.mat.channels() == 4) {
		cv::cvtColor(matInfo.mat, matInfo.mat, cv::COLOR_BGRA2BGR);
	}
	if (matInfo.mat.type() != CV_8UC3) {
		matInfo.mat.convertTo(matInfo.mat, CV_8UC3);
	}

	if (matInfo.mat.empty()) {
		return; // 跳过空帧
	}

	auto isModelNeedGet = MDOFoodBags::getIsModelImageLoaded();
	if (!isModelNeedGet)
	{
		HalconCpp::HObject hImage = rw::rqw::CvMatToHImage(matInfo.mat);
		HalconCpp::Rgb1ToGray(hImage, &hImage);
		HalconCpp::MeanImage(hImage, &hImage, 3, 3);
		MDOFoodBags::setModelImage(hImage);
		MDOFoodBags::setIsModelImageLoaded(true);
	}

	QMutexLocker locker(&_mutex);
	MatInfo mat;
	mat.image = matInfo.mat;
	mat.index = index;

	mat.location = loc;

	_queue.enqueue(mat);
	_condition.wakeOne();
}
void ImageProcessor::drawLimitLines(QImage& image, double leftLimit, double rightLimit, const QColor& color, int penWidth)
{
	// 检查图像是否有效
	if (image.isNull())
	{
		return;
	}

	// 创建 QPainter 对象用于在图像上绘制
	QPainter painter(&image);

	// 设置抗锯齿,使绘制的线条边缘更平滑
	painter.setRenderHint(QPainter::Antialiasing, true);

	// 设置画笔颜色和宽度
	QPen pen(color);
	pen.setWidth(penWidth);
	pen.setStyle(Qt::DashLine);  // 使用虚线样式
	painter.setPen(pen);

	int imageHeight = image.height();

	// 绘制左限位线 (垂直线)
	int leftX = static_cast<int>(leftLimit);
	painter.drawLine(leftX, 0, leftX, imageHeight);

	// 绘制右限位线 (垂直线)
	int rightX = static_cast<int>(rightLimit);
	painter.drawLine(rightX, 0, rightX, imageHeight);

	// 绘制限位线标签
	QFont font = painter.font();
	font.setPointSize(25);
	font.setBold(true);
	painter.setFont(font);

	// 左限位标签
	QString leftText = QString("左限位: %1").arg(leftLimit, 0, 'f', 0);
	QFontMetrics metrics(font);
	QRect leftTextRect = metrics.boundingRect(leftText);
	leftTextRect.moveTo(leftX + 10, 50);
	leftTextRect.adjust(-5, -5, 5, 5);

	// 绘制半透明背景
	painter.fillRect(leftTextRect, QColor(0, 0, 0, 180));

	// 绘制文本(白色)
	painter.setPen(Qt::white);
	painter.drawText(leftTextRect, Qt::AlignCenter, leftText);

	// 右限位标签
	QString rightText = QString("右限位: %1").arg(rightLimit, 0, 'f', 0);
	QRect rightTextRect = metrics.boundingRect(rightText);
	rightTextRect.moveTo(rightX - rightTextRect.width() - 10, 50);
	rightTextRect.adjust(-5, -5, 5, 5);

	// 绘制半透明背景
	painter.fillRect(rightTextRect, QColor(0, 0, 0, 180));

	// 绘制文本(白色)
	painter.drawText(rightTextRect, Qt::AlignCenter, rightText);

	// 结束绘制
	painter.end();
}