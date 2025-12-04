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

	emit imageNGReady(QPixmap::fromImage(image), frame.index, false, false);

	/*rw::rqw::ImageInfo imageInfo(image);
	save_image(imageInfo, image, 1);*/
}

void ImageProcessor::run_OpenRemoveFunc(MatInfo& frame)
{
	auto startTime = std::chrono::high_resolution_clock::now();

	auto image = rw::rqw::cvMatToQImage(frame.image);
	auto saveRowImg = image.copy();

	//显示图片测试
	//QString imagePath = "C:/Users/zfkj4090/Desktop/temp/image.jpg";
	//QImage image(imagePath);
	//进行图像处理
	//4个qvector用来存坏的数据的位置，还有面积
	QVector< MatProcess> _matProcess;
	MatProduct _matProduct;
	halconPRocess(frame.image, _matProcess, _matProduct);

	double minArea = 0;
	double allMinArea = 0;

	//这个函数可以判断是不是坏的，并且在图像上画出矩形，还要绘制左右限位
	_isbad = checkDefectAndDrawOnImage(image, _matProcess, minArea, allMinArea,_matProduct);
	run_OpenRemoveFunc_emitErrorInfo(_isbad);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	double processingTimeMs = duration.count();

	drawProcessingTime(image, processingTimeMs);

	emit imageNGReady(QPixmap::fromImage(image), frame.index, false, false);

	rw::rqw::ImageInfo imageInfo(image);
	save_image(imageInfo, image, 1);

	rw::rqw::ImageInfo imageInfo1(saveRowImg);
	save_image(imageInfo1, saveRowImg, 2);
}

void ImageProcessor::halconPRocess(cv::Mat image, QVector<MatProcess>& processResults, MatProduct& _matProduct)
{
	// 清空结果容器
	processResults.clear();

	// 模板图片
	  // Local iconic variables
	HalconCpp::HObject  ho_ImageModel, ho_Region, ho_ConnectedRegions1;
	HalconCpp::HObject  ho_SelectedRegions1, ho_RegionTrans1, ho_Image;
	HalconCpp::HObject  ho_ImageModelMean, ho_ImageMean, ho_ImageSub, ho_Rectangle;
	HalconCpp::HObject  ho_ImageReduced, ho_Regions, ho_ConnectedRegions;
	HalconCpp::HObject  ho_RegionTrans, ho_ObjectSelected;

	// Local control variables
	HalconCpp::HTuple  hv_UsedThreshold, hv_shapetransRow11;
	HalconCpp::HTuple  hv_shapetransColumn11, hv_shapetransRow22, hv_shapetransColumn22;
	HalconCpp::HTuple  hv_modelzuoceduiqi, hv_Width, hv_Height, hv_marskx;
	HalconCpp::HTuple  hv_marsky, hv_zuoxianwei, hv_youxianwei, hv_zuoyousuojin;
	HalconCpp::HTuple  hv_huidumin, hv_iswenchong, hv_wenchongArea, hv_wenchonghuidu;
	HalconCpp::HTuple  hv_wenchongkaundu, hv_wenchongchangdu, hv_istoufa;
	HalconCpp::HTuple  hv_toufaArea, hv_toufahuidu, hv_toufakuandu, hv_toufachangdu;
	HalconCpp::HTuple  hv_kuandu, hv_zuoceduiqi, hv_HomMat2D, hv_Area;
	HalconCpp::HTuple  hv_Row, hv_Column, hv_shapetransRow1, hv_shapetransColumn1;
	HalconCpp::HTuple  hv_shapetransRow2, hv_shapetransColumn2, hv_Index;
	HalconCpp::HTuple  hv_a, hv_shangxiasuojin, hv_Mean, hv_Deviation;
	double xiangsudangliang = Modules::getInstance().configManagerModule.setConfig.xiangSuDangLiang;

	// 需要认为设置的参数现在硬编码了
	hv_marskx = 9;
	hv_marsky = 9;
	hv_zuoxianwei = Modules::getInstance().configManagerModule.setConfig.zuoXianWei;
	hv_youxianwei = Modules::getInstance().configManagerModule.setConfig.youXianWei;
	hv_shangxiasuojin = Modules::getInstance().configManagerModule.setConfig.shangxiasuojin;

	hv_huidumin = 100;

	// 获取模板图像
	auto modelImagePtr = MDOFoodBags::getModelHImage();
	if (modelImagePtr && modelImagePtr->IsInitialized()) {
		ho_ImageModel = *modelImagePtr;
	}

	// 当前图片
	ho_Image = rw::rqw::CvMatToHImage(image);
	HalconCpp::Rgb1ToGray(ho_Image, &ho_Image);

	// 获取图片尺寸
	GetImageSize(ho_Image, &hv_Width, &hv_Height);

	if (!modelImagePtr || !modelImagePtr->IsInitialized()) {
		return;
	}
	//计算模板图像左侧位置让所有图片都匹配最左侧位置
	MeanImage(ho_ImageModel, &ho_ImageModelMean, hv_marskx, hv_marsky);
	BinaryThreshold(ho_ImageModel, &ho_Region, "max_separability", "dark", &hv_UsedThreshold);
	Connection(ho_Region, &ho_ConnectedRegions1);
	SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
	ShapeTrans(ho_SelectedRegions1, &ho_RegionTrans1, "rectangle1");
	SmallestRectangle1(ho_RegionTrans1, &hv_shapetransRow11, &hv_shapetransColumn11,
		&hv_shapetransRow22, &hv_shapetransColumn22);

	hv_modelzuoceduiqi = hv_shapetransColumn11;






	MeanImage(ho_Image, &ho_ImageMean, hv_marskx, hv_marsky);


	//求袋子的宽度

	BinaryThreshold(ho_Image, &ho_Region, "max_separability", "dark", &hv_UsedThreshold);
	Connection(ho_Region, &ho_ConnectedRegions1);
	SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
	ShapeTrans(ho_SelectedRegions1, &ho_RegionTrans1, "rectangle1");
	SmallestRectangle1(ho_RegionTrans1, &hv_shapetransRow11, &hv_shapetransColumn11,
		&hv_shapetransRow22, &hv_shapetransColumn22);
	


	if (0 != (int((hv_kuandu.TupleLength()) > 0)))
	{
		//TODO:缩进改为可设值
		hv_zuoxianwei = hv_shapetransColumn11 + hv_zuoyousuojin;
		hv_youxianwei = hv_shapetransColumn22 - hv_zuoyousuojin;
		hv_kuandu = hv_shapetransColumn22 - hv_shapetransColumn11;
		hv_zuoceduiqi = hv_shapetransColumn11;
		_matProduct.Width = hv_kuandu.D() * xiangsudangliang;
		_matProduct.zuoxianwei = hv_zuoxianwei.D();
		_matProduct.youxianwei = hv_youxianwei.D();

	}
	else
	{
		hv_zuoxianwei = Modules::getInstance().configManagerModule.setConfig.zuoXianWei;
		hv_youxianwei = Modules::getInstance().configManagerModule.setConfig.youXianWei;
		hv_zuoceduiqi = 0;
		_matProduct.Width = (hv_youxianwei- hv_zuoxianwei).D() * xiangsudangliang;
		_matProduct.zuoxianwei = hv_zuoxianwei.D();
		_matProduct.youxianwei = hv_youxianwei.D();


	}


	VectorAngleToRigid(0, hv_zuoceduiqi, 0, 0, hv_modelzuoceduiqi, 0, &hv_HomMat2D);
	AffineTransImage(ho_ImageMean, &ho_ImageMean, hv_HomMat2D, "constant", "false");










	SubImage(ho_ImageModelMean, ho_ImageMean, &ho_ImageSub, 10, 0);

	if (hv_Height < hv_shangxiasuojin)
	{
		hv_shangxiasuojin = 0;
	}










	GenRectangle1(&ho_Rectangle, 20, hv_zuoxianwei, hv_Height - 20, hv_youxianwei);
	ReduceDomain(ho_ImageSub, ho_Rectangle, &ho_ImageReduced);

	Threshold(ho_ImageReduced, &ho_Regions, hv_huidumin, 255);
	HalconCpp::Connection(ho_Regions, &ho_Regions);
	AreaCenter(ho_Regions, &hv_Area, &hv_Row, &hv_Column);

	ShapeTrans(ho_Regions, &ho_RegionTrans, "rectangle1");
	SmallestRectangle1(ho_RegionTrans, &hv_shapetransRow1, &hv_shapetransColumn1, &hv_shapetransRow2,
		&hv_shapetransColumn2);

	if (0 != (int((hv_shapetransRow1.TupleLength()) > 0)))
	{
		HalconCpp::HTuple end_val39 = (hv_shapetransRow1.TupleLength()) - 1;
		HalconCpp::HTuple step_val39 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val39, step_val39); hv_Index += step_val39)
		{
			if (hv_shapetransRow1[hv_Index] > 0)
			{
				SelectObj(ho_Regions, &ho_ObjectSelected, hv_Index + 1);
				Intensity(ho_ObjectSelected, ho_ImageSub, &hv_Mean, &hv_Deviation);
				MatProcess result;
				result.R1 = hv_shapetransRow1[hv_Index];
				result.C1 = hv_shapetransColumn1[hv_Index];
				result.R2 = hv_shapetransRow2[hv_Index];
				result.C2 = hv_shapetransColumn2[hv_Index];
				result.Area = hv_Area[hv_Index] * xiangsudangliang * xiangsudangliang;
				result.MeanThreshold = hv_Mean;
				

				processResults.append(result);
			}
		}
	}
}
void ImageProcessor::drawSingleRectangleOnImage(QImage& image,
	const MatProcess& result,
	const QColor& color,
	int penWidth)
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
	int x1 = static_cast<int>(result.C1);
	int y1 = static_cast<int>(result.R1);
	int x2 = static_cast<int>(result.C2);
	int y2 = static_cast<int>(result.R2);

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
	QString areaText = QString("%1 mm²").arg(result.Area, 0, 'f', 2);

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
void ImageProcessor::drawRectanglesOnImage(QImage& image,
	const QVector<MatProcess>& processResults,
	const QColor& color,
	int penWidth)
{
	// 检查图像是否有效
	if (image.isNull())
	{
		return;
	}

	// 如果没有矩形需要绘制,直接返回
	if (processResults.isEmpty())
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
	for (const auto& result : processResults)
	{
		totalArea += result.Area;
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
	for (const auto& result : processResults)
	{
		// Halcon 坐标系: R 表示 Row (行,对应 y 坐标), C 表示 Column (列,对应 x 坐标)
		// R1, C1 是矩形左上角坐标
		// R2, C2 是矩形右下角坐标
		int x1 = static_cast<int>(result.C1);
		int y1 = static_cast<int>(result.R1);
		int x2 = static_cast<int>(result.C2);
		int y2 = static_cast<int>(result.R2);

		// 计算矩形的宽度和高度
		int width = x2 - x1;
		int height = y2 - y1;

		// 绘制矩形框
		painter.drawRect(x1, y1, width, height);
	}

	// 结束绘制
	painter.end();
}

void ImageProcessor::drawDefectInfo(QImage& image, double area, double meanThreshold)
{
	// 检查图像是否有效
	if (image.isNull())
	{
		return;
	}

	// 创建 QPainter 对象用于在图像上绘制
	QPainter painter(&image);

	// 设置抗锯齿
	painter.setRenderHint(QPainter::Antialiasing, true);

	// 设置字体
	QFont font = painter.font();
	font.setPointSize(6);  // 字体大小
	font.setBold(false);     // 加粗
	painter.setFont(font);

	// 准备文本内容
	QStringList textLines;
	textLines << QString("面积: %1 mm²").arg(area, 0, 'f', 2);
	textLines << QString("灰度值: %1").arg(meanThreshold, 0, 'f', 2);

	// 计算文本位置
	QFontMetrics metrics(font);
	int lineHeight = metrics.height();
	int maxWidth = 0;

	// 计算最大文本宽度
	for (const QString& line : textLines)
	{
		int width = metrics.horizontalAdvance(line);
		if (width > maxWidth)
		{
			maxWidth = width;
		}
	}

	// 创建文本区域矩形（位于左上角）
	int padding = 5;
	QRect textRect(10, 10, maxWidth + 2 * padding, textLines.size() * lineHeight + 2 * padding);

	// 绘制半透明背景
	painter.fillRect(textRect, QColor(0, 0, 0, 180));

	// 绘制文本（白色）
	painter.setPen(Qt::white);
	int yPos = textRect.top() + padding + metrics.ascent();
	for (const QString& line : textLines)
	{
		painter.drawText(textRect.left() + padding, yPos, line);
		yPos += lineHeight;
	}

	// 结束绘制
	painter.end();
}
bool ImageProcessor::checkDefectAndDrawOnImage(
	QImage& image,
	const QVector<MatProcess>& processResults,
	double minArea,
	double allMinArea,
	MatProduct _matProduct)
{
	bool isbad = false;

	// 绘制左右限位
	double zuoxianwei = Modules::getInstance().configManagerModule.setConfig.zuoXianWei;
	double youxianwei = Modules::getInstance().configManagerModule.setConfig.youXianWei;
	if (_matProduct.zuoxianwei>0)
	{
		zuoxianwei = _matProduct.zuoxianwei;
		youxianwei = _matProduct.youxianwei;


	}
	//TODO:绘制限位线






	// 说明有瑕疵
	if (processResults.size() > 0)
	{
		// 计算总面积
		double totalArea = 0.0;
		for (const auto& result : processResults)
		{
			totalArea += result.Area;
		}

		// 判断是否为坏袋子:
		// 1. 总面积超过最小总面积阈值
		// 2. 或者存在单个面积超过最小面积阈值

		// 检查是否有单个面积超过阈值并绘制
		auto paintimage = image.copy();
		for (const auto& result : processResults)
		{
			if (result.Area >= minArea)
			{
				drawSingleRectangleOnImage(image, result, Qt::red, 3);

				// 提取缺陷区域图像
				QImage defectRegion = extractDefectRegion(paintimage, result);

				if (!defectRegion.isNull())
				{
					
					// 绘制面积和灰度值在图片上
					drawDefectInfo(defectRegion, result.Area, result.MeanThreshold);



					// 保存缺陷区域图像
					rw::rqw::ImageInfo defectInfo(defectRegion);
					defectInfo.classify = QString("Defect1");
					auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
					imageSaveEngine->pushImage(defectInfo);

					emit imageNGReady(QPixmap::fromImage(defectRegion), 1, true, true);
				}

				isbad = true;
			}
			else
			{
				drawSingleRectangleOnImage(image, result, Qt::green, 3);
			}
		}

		// 检查总面积是否超过阈值
		if (totalArea >= allMinArea)
		{
			isbad = true;

			// 在图像上绘制检测到的矩形区域
			// 红色表示缺陷区域，线宽为3像素
			drawRectanglesOnImage(image, processResults, Qt::red, 3);
		}
	}

	// 绘制限位线
	drawLimitLines(image, zuoxianwei, youxianwei, Qt::yellow, 3);

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

void ImageProcessor::save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image, int imageIndex)
{
	auto& isTakePictures = Modules::getInstance().runtimeInfoModule.isTakePictures;

	if (!isTakePictures)
	{
		return;
	}

	if ((imageProcessingModuleIndex == 1))
	{
		save_image_work(imageInfo, image, imageIndex);
	}
}

void ImageProcessor::save_image_work(rw::rqw::ImageInfo& imageInfo, const QImage& image, int imageIndex)
{
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
	if (_isbad) {

		if (1 == imageProcessingModuleIndex)
		{
			if (1 == imageIndex)
			{
				imageInfo.classify = "NG1";
				imageSaveEngine->pushImage(imageInfo);
			}
			else if (2 == imageIndex)
			{
				imageInfo.classify = "OK1";
				imageSaveEngine->pushImage(imageInfo);
			}
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
		MDOFoodBags::setModelHImage(hImage);
		MDOFoodBags::setIsModelImageLoaded(true);
		QImage modelQImage = rw::rqw::cvMatToQImage(matInfo.mat);
		MDOFoodBags::setModelQImage(modelQImage);

		// 保存模板图片
		auto qImage = rw::rqw::cvMatToQImage(matInfo.mat);

		rw::rqw::ImageInfo imageInfo(qImage);
		auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;

		imageInfo.classify = "Module1";
		imageSaveEngine->pushImage(imageInfo);

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

QImage ImageProcessor::extractDefectRegion(const QImage& sourceImage,
	const MatProcess& result,
	int minSize)
{
	// 检查源图像是否有效
	if (sourceImage.isNull())
	{
		return QImage();
	}

	// 转换坐标系: R(行/y), C(列/x)
	int x1 = static_cast<int>(result.C1);
	int y1 = static_cast<int>(result.R1);
	int x2 = static_cast<int>(result.C2);
	int y2 = static_cast<int>(result.R2);

	// 确保坐标顺序正确
	if (x1 > x2) {
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if (y1 > y2) {
		int temp = y1;
		y1 = y2;
		y2 = temp;
	}

	// 计算原始区域的宽度和高度
	int width = x2 - x1;
	int height = y2 - y1;

	// 获取图像尺寸
	int imageWidth = sourceImage.width();
	int imageHeight = sourceImage.height();

	// 计算区域中心点
	int centerX = (x1 + x2) / 2;
	int centerY = (y1 + y2) / 2;

	// 确定正方形的边长(取宽高中的较大值,并确保不小于minSize)
	int sideLength = width > height ? width : height;
	sideLength = sideLength > minSize ? sideLength : minSize;

	// 以中心点为基准计算正方形的坐标
	int halfSide = sideLength / 2;
	x1 = centerX - halfSide;
	x2 = centerX + halfSide;
	y1 = centerY - halfSide;
	y2 = centerY + halfSide;

	// 边界检查和调整,确保不超出图像范围
	if (x1 < 0)
	{
		int offset = -x1;
		x1 = 0;
		x2 = (x2 + offset < imageWidth) ? (x2 + offset) : imageWidth;
	}
	if (y1 < 0)
	{
		int offset = -y1;
		y1 = 0;
		y2 = (y2 + offset < imageHeight) ? (y2 + offset) : imageHeight;
	}
	if (x2 > imageWidth)
	{
		int offset = x2 - imageWidth;
		x2 = imageWidth;
		x1 = (x1 - offset > 0) ? (x1 - offset) : 0;
	}
	if (y2 > imageHeight)
	{
		int offset = y2 - imageHeight;
		y2 = imageHeight;
		y1 = (y1 - offset > 0) ? (y1 - offset) : 0;
	}

	// 重新计算宽高
	width = x2 - x1;
	height = y2 - y1;

	// 确保宽高为正数
	if (width <= 0 || height <= 0)
	{
		return QImage();
	}

	// 提取图像区域
	QRect extractRect(x1, y1, width, height);
	QImage extractedImage = sourceImage.copy(extractRect);

	return extractedImage;
}

void ImageProcessor::drawProcessingTime(QImage& image, double timeMs, const QColor& backgroundColor,
	const QColor& textColor)
{
	// 检查图像是否有效
	if (image.isNull())
	{
		return;
	}

	// 创建 QPainter 对象用于在图像上绘制
	QPainter painter(&image);

	// 设置抗锯齿
	painter.setRenderHint(QPainter::Antialiasing, true);

	// 设置字体
	QFont font = painter.font();
	font.setPointSize(24);  // 字体大小
	font.setBold(true);     // 加粗
	painter.setFont(font);

	// 准备文本内容
	QString timeText = QString("处理时间: %1 ms").arg(timeMs, 0, 'f', 2);

	// 计算文本位置
	QFontMetrics metrics(font);
	QRect textRect = metrics.boundingRect(timeText);

	// 放置在左上角，留10像素边距
	textRect.moveTo(10, 10);
	textRect.adjust(-5, -5, 5, 5);  // 扩展矩形边距

	// 绘制半透明背景
	painter.fillRect(textRect, backgroundColor);

	// 绘制文本
	painter.setPen(textColor);
	painter.drawText(textRect, Qt::AlignCenter, timeText);

	// 结束绘制
	painter.end();
}
