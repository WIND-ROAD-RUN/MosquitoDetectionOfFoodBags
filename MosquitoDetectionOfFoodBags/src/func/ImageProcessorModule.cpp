#include "ImageProcessorModule.hpp"

#include "MDOFoodBags.h"
#include "Modules.hpp"
#include "rqw_HalconUtilty.hpp"
#include "Utilty.hpp"
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <cmath>

#include "osoFIleUtiltyFunc.hpp"

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
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	defectLoc = 0.0;

	auto image = rw::rqw::cvMatToQImage(frame.image);
	auto saveRowImg = image.copy();

	//进行图像处理
	//4个qvector用来存坏的数据的位置，还有面积
	QVector< MatProcess> _matProcess;
	MatProduct _matProduct;
	halconPRocess(frame.image, _matProcess, _matProduct);

	double minArea = 0;
	double allMinArea = 0;

	minArea = setConfig.wenchongzuixiaomianji;
	allMinArea = minArea;


	//这个函数可以判断是不是坏的，并且在图像上画出矩形，还要绘制左右限位
	_isbad = checkDefectAndDrawOnImage(image, _matProcess, minArea, allMinArea, _matProduct);

	bool hasWenChong = false;
	bool hasMaoFa = false;

	for (const auto& item : _matProcess)
	{
		// 计算剔废位置
		auto tempLoc = item.location;
		if (tempLoc > static_cast<double>(defectLoc))
		{
			defectLoc = static_cast<float>(tempLoc);
		}
		// 总结剔废类型用于报警
		if (0 == item.classid)
		{
			hasWenChong = true;
		}
		if (1 == item.classid)
		{
			hasMaoFa = true;
		}
	}

	// 更新报警信息
	auto& runtimeInfoModule = Modules::getInstance().runtimeInfoModule;
	// 0: 无/仅蚊虫(0) / 仅毛发(1) / 两者都有(2)
	const int defectMask = (hasWenChong ? 1 : 0) | (hasMaoFa ? 2 : 0);
	runtimeInfoModule.lastDefectClassId.store(
		(defectMask == 1) ? 0 :
		(defectMask == 2) ? 1 :
		(defectMask == 3) ? 2 : -1);

	defectLoc += frame.location;
	defectLoc += setConfig.tifeijuli;

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
	HalconCpp::HObject  ho_SelectedRegions, ho_ImageMean1, ho_ImageCleared;

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
	HalconCpp::HTuple  hv_a, hv_shangxiasuojin, hv_Mean, hv_Deviation, R1, R2, C1, C2;

	HalconCpp::HObject ho_image1, ho_image2, ho_ImageSub1, ho_ImageSub2, ho_ImageReduced1, ho_ImageReduced2;
	HalconCpp::HObject ho_Regions1, ho_Regions2;

	HalconCpp::HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;




	double xiangsudangliang = Modules::getInstance().configManagerModule.setConfig.xiangSuDangLiang;

	// 需要认为设置的参数现在硬编码了
	hv_marskx = 9;
	hv_marsky = 9;
	hv_zuoxianwei = Modules::getInstance().configManagerModule.setConfig.zuoXianWei;
	hv_youxianwei = Modules::getInstance().configManagerModule.setConfig.youXianWei;
	hv_shangxiasuojin = Modules::getInstance().configManagerModule.setConfig.shangxiasuojin;

	hv_huidumin = Modules::getInstance().configManagerModule.setConfig.wenchongzuixiaohuiduchazhi;


	// 当前图片
	ho_Image = rw::rqw::CvMatToHImage(image);
	HalconCpp::Rgb1ToGray(ho_Image, &ho_Image);


	// 获取图片尺寸
	//创建模板
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	MeanImage(ho_Image, &ho_ImageMean, hv_marskx, hv_marsky);

	double w = hv_Width;
	double h = hv_Height;


	BinaryThreshold(ho_Image, &ho_Region, "max_separability", "dark", &hv_UsedThreshold);
	Connection(ho_Region, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);

	HalconCpp::AreaCenter(ho_SelectedRegions, &hv_Area, &hv_Row, &hv_Column);
	double area = hv_Area;
	double ll = hv_Area.TupleLength();
	if (0 == (int((hv_Area.TupleLength()) > 0)))
	{
		return;
	}
	//拆分两个不一样的灰度计算方法进行差分

	CopyImage(ho_ImageMean, &ho_image1);
	CopyImage(ho_ImageMean, &ho_image2);
	SmallestRectangle1(ho_SelectedRegions, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);


	double quyukuaishu = 3;

	double kuandu = hv_Column2 - hv_Column1;
	double dankuandu = kuandu / quyukuaishu;
	dankuandu += 1;

	{
		HalconCpp::HTuple end_val39 = quyukuaishu;
		HalconCpp::HTuple step_val39 = 1;
		HalconCpp::HTuple hv_Index1;
		for (hv_Index1 = 0; hv_Index1.Continue(end_val39, step_val39); hv_Index1 += step_val39)
		{
			HalconCpp::HObject ho_Rectangle1;

			GenRectangle1(&ho_Rectangle1, hv_Row1, (hv_Column1 + (hv_Index1 * dankuandu)) - 1,
				hv_Row2, (hv_Column1 + (dankuandu * (hv_Index1 + 1))) + 1);
			Intensity(ho_Rectangle1, ho_ImageMean, &hv_Mean, &hv_Deviation);
			OverpaintRegion(ho_image1, ho_Rectangle1, hv_Mean, "fill");

		}
	}
	quyukuaishu = 10;
	kuandu = hv_Column2 - hv_Column1;
	dankuandu = kuandu / quyukuaishu;
	dankuandu += 1;

	{
		HalconCpp::HTuple end_val51 = quyukuaishu;
		HalconCpp::HTuple step_val51 = 1; HalconCpp::HTuple hv_Index1;
		for (hv_Index1 = 0; hv_Index1.Continue(end_val51, step_val51); hv_Index1 += step_val51)
		{
			HalconCpp::HObject ho_Rectangle1;

			GenRectangle1(&ho_Rectangle1, hv_Row1, (hv_Column1 + (hv_Index1 * dankuandu)) - 1,
				hv_Row2, (hv_Column1 + (dankuandu * (hv_Index1 + 1))) + 1);
			Intensity(ho_Rectangle1, ho_ImageMean, &hv_Mean, &hv_Deviation);
			OverpaintRegion(ho_image2, ho_Rectangle1, hv_Mean, "fill");

		}
	}






	ShapeTrans(ho_SelectedRegions, &ho_RegionTrans1, "rectangle1");
	SmallestRectangle1(ho_RegionTrans1, &R1, &hv_zuoxianwei, &R2, &hv_youxianwei);




	if (0 != (int((hv_zuoxianwei.TupleLength()) > 0)))
	{
		//获取图像里面的宽度
		_matProduct.Width = (hv_youxianwei - hv_zuoxianwei).D() * xiangsudangliang;
		_matProduct.zuoxianwei = hv_zuoxianwei.D();
		_matProduct.youxianwei = hv_youxianwei.D();


	}
	else
	{
		return;
	}





	SubImage(ho_image1, ho_ImageMean, &ho_ImageSub1, 10, 0);
	SubImage(ho_image2, ho_ImageMean, &ho_ImageSub2, 10, 0);
	ho_ImageSub = ho_ImageSub1;

	if (hv_Height < hv_shangxiasuojin)
	{
		hv_shangxiasuojin = 0;
	}






	double z = hv_zuoxianwei.TupleLength();
	double y = hv_youxianwei.TupleLength();

	GenRectangle1(&ho_Rectangle, hv_shangxiasuojin, hv_zuoxianwei, hv_Height - hv_shangxiasuojin, hv_youxianwei);

	ReduceDomain(ho_ImageSub1, ho_Rectangle, &ho_ImageReduced1);
	ReduceDomain(ho_ImageSub2, ho_Rectangle, &ho_ImageReduced2);

	//判断蚊虫
	{
		

		double zuixiaohuidu = Modules::getInstance().configManagerModule.setConfig.wenchongzuixiaohuiduchazhi;
		double zuidahuidu = Modules::getInstance().configManagerModule.setConfig.wenchongzuidahuiduchazhi;

		if (zuixiaohuidu > zuidahuidu)
		{
			zuixiaohuidu = 50;
			zuidahuidu = 255;

		}
		Threshold(ho_ImageReduced1, &ho_Regions1, zuixiaohuidu, zuidahuidu);

		Threshold(ho_ImageReduced2, &ho_Regions2, zuixiaohuidu, zuidahuidu);

		Intersection(ho_Regions1, ho_Regions2, &ho_Regions);



		HalconCpp::Connection(ho_Regions, &ho_Regions);
		AreaCenter(ho_Regions, &hv_Area, &hv_Row, &hv_Column);

		ShapeTrans(ho_Regions, &ho_RegionTrans, "rectangle1");
		SmallestRectangle1(ho_RegionTrans, &hv_shapetransRow1, &hv_shapetransColumn1, &hv_shapetransRow2,
			&hv_shapetransColumn2);
		double we = hv_shapetransRow1.TupleLength();
		double are = zuixiaohuidu;

		if (0 != (int((hv_shapetransRow1.TupleLength()) > 0)))
		{
			QVector<MatProcess> tempResults;

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
					result.location = (result.R2 - result.R1) * xiangsudangliang;

					if (result.Area > Modules::getInstance().configManagerModule.setConfig.wenchongzuixiaomianji && result.Area < Modules::getInstance().configManagerModule.setConfig.wenchongzuidamianji)
					{
						result.classid = 0; // 蚊虫
						tempResults.append(result);

					}

					else
					{
						result.classid = -1; // 未知瑕疵
					}

				}
			}

			// 仅保留最大的10个
			if (tempResults.size() > 10)
			{
				std::sort(tempResults.begin(), tempResults.end(),
					[](const MatProcess& a, const MatProcess& b) {
						return a.Area > b.Area;
					});
				tempResults.resize(10);
			}

			processResults += tempResults;
		}

	}


	//判断毛发
	{
	

		double zuixiaohuidu = Modules::getInstance().configManagerModule.setConfig.maofazuixiaohuiduchazhi;
		double zuidahuidu = Modules::getInstance().configManagerModule.setConfig.maofazuidahuiduchazhi;

		if (zuixiaohuidu> zuidahuidu)
		{
			zuixiaohuidu = 50;
			zuidahuidu = 255;

		}
		Threshold(ho_ImageReduced1, &ho_Regions1, zuixiaohuidu, zuidahuidu);

		Threshold(ho_ImageReduced2, &ho_Regions2, zuixiaohuidu, zuidahuidu);

		Intersection(ho_Regions1, ho_Regions2, &ho_Regions);



		HalconCpp::Connection(ho_Regions, &ho_Regions);
		AreaCenter(ho_Regions, &hv_Area, &hv_Row, &hv_Column);

		ShapeTrans(ho_Regions, &ho_RegionTrans, "rectangle1");
		SmallestRectangle1(ho_RegionTrans, &hv_shapetransRow1, &hv_shapetransColumn1, &hv_shapetransRow2,
			&hv_shapetransColumn2);
		double we = hv_shapetransRow1.TupleLength();
		double are = zuixiaohuidu;

		if (0 != (int((hv_shapetransRow1.TupleLength()) > 0)))
		{
			QVector<MatProcess> tempResults;

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
					result.location = (result.R2 - result.R1) * xiangsudangliang;

					if (result.Area > Modules::getInstance().configManagerModule.setConfig.maofazuixiaomianji && result.Area < Modules::getInstance().configManagerModule.setConfig.maofazuidamianji)
					{
						result.classid =1; // 蚊虫
					 tempResults.append(result);

					}

					else
					{
						result.classid = -1; // 未知瑕疵
					}

				}
			}

			// 仅保留最大的10个
			if (tempResults.size() > 10)
			{
				std::sort(tempResults.begin(), tempResults.end(),
					[](const MatProcess& a, const MatProcess& b) {
						return a.Area > b.Area;
					});
				tempResults.resize(10);
			}

			processResults += tempResults;
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

	QPainter painter(&image);
	painter.setRenderHint(QPainter::Antialiasing, true);

	QPen pen(color);
	pen.setWidth(penWidth);
	painter.setPen(pen);

	int x1 = static_cast<int>(result.C1);
	int y1 = static_cast<int>(result.R1);
	int x2 = static_cast<int>(result.C2);
	int y2 = static_cast<int>(result.R2);

	// 确保坐标顺序正确
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);

	int width = x2 - x1;
	int height = y2 - y1;
	if (width <= 0 || height <= 0)
	{
		return;
	}

	// 在中心点不变的前提下放大 20%
	const double scale = 1;
	int centerX = x1 + width / 2;
	int centerY = y1 + height / 2;

	int newWidth = static_cast<int>(std::round(width * scale));
	int newHeight = static_cast<int>(std::round(height * scale));

	int drawX1 = centerX - newWidth / 2;
	int drawY1 = centerY - newHeight / 2;
	int drawX2 = drawX1 + newWidth;
	int drawY2 = drawY1 + newHeight;

	// 边界裁剪，避免画出图外
	drawX1 = (drawX1 < 0) ? 0 : drawX1;
	drawY1 = (drawY1 < 0) ? 0 : drawY1;

	const int maxX = image.width() - 1;
	const int maxY = image.height() - 1;

	drawX2 = (drawX2 > maxX) ? maxX : drawX2;
	drawY2 = (drawY2 > maxY) ? maxY : drawY2;

	int drawW = drawX2 - drawX1;
	int drawH = drawY2 - drawY1;
	if (drawW <= 0 || drawH <= 0)
	{
		return;
	}

	// 绘制放大后的矩形框
	painter.drawRect(drawX1, drawY1, drawW, drawH);

	// 绘制面积文本（使用放大后的框来居中计算）
	QFont font = painter.font();
	font.setPointSize(30);
	font.setBold(true);
	painter.setFont(font);

	QString areaText = QString("%1 mm²").arg(result.Area, 0, 'f', 2);

	QFontMetrics metrics(font);
	QRect textRect = metrics.boundingRect(areaText);

	int textX = drawX1 + (drawW - textRect.width()) / 2;
	int textY = drawY1 - 10;

	if (textY - textRect.height() < 0)
	{
		textY = drawY1 + textRect.height() + 10;
	}

	textRect.moveTo(textX, textY - textRect.height());
	textRect.adjust(-5, -5, 5, 5);

	painter.fillRect(textRect, QColor(0, 0, 0, 180));
	painter.setPen(Qt::white);
	painter.drawText(textRect, Qt::AlignCenter, areaText);

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
	if (_matProduct.zuoxianwei > 0)
	{
		zuoxianwei = _matProduct.zuoxianwei;
		youxianwei = _matProduct.youxianwei;


	}
	//TODO:绘制限位线






	// 说明有瑕疵
	if (processResults.size() > 0)
	{
		
		// 判断是否为坏袋子:
		// 1. 总面积超过最小总面积阈值
		// 2. 或者存在单个面积超过最小面积阈值

		// 检查是否有单个面积超过阈值并绘制
		auto paintimage = image.copy();
		for (const auto& result : processResults)
		{
			
				drawSingleRectangleOnImage(image, result, Qt::red, 1);

				// 提取缺陷区域图像
				QImage defectRegion = extractDefectRegion(paintimage, result);

				if (!defectRegion.isNull())
				{

					// 绘制面积和灰度值在图片上
					//drawDefectInfo(defectRegion, result.Area, result.MeanThreshold);



					// 保存缺陷区域图像
					rw::rqw::ImageInfo defectInfo(defectRegion);
					defectInfo.classify = QString("Defect1");
					auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
					imageSaveEngine->pushImage(defectInfo);

					emit imageNGReady(QPixmap::fromImage(defectRegion), 1, true, true);
				}

				isbad = true;
			
			
		}

		
	}

	// 绘制限位线
	drawLimitLines(image, zuoxianwei, youxianwei, Qt::yellow, 3);

	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	auto pixBagLength = (youxianwei - zuoxianwei) * setConfig.xiangSuDangLiang;
	statisticalInfo.bagLength = static_cast<uint64_t>(std::round(pixBagLength));

	return isbad;
}





void ImageProcessor::run_OpenRemoveFunc_emitErrorInfo(bool isbad)
{
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& mainWindowConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;
	auto& priorityQueue1 = Modules::getInstance().eliminateModule.productPriorityQueue1;

	if (1 != imageProcessingModuleIndex)
	{
		return;
	}

	if (isbad)
	{
		++statisticalInfo.wasteCount;
	}

	if (imageProcessingModuleIndex == 1)
	{
		// 生产长度统计，单位毫米
		statisticalInfo.productionLength += static_cast<uint64_t>(setConfig.xiangjichufachangdu);
	}

	// =========================
	// 状态机：needGoodReset == true 表示“锁定(禁止剔废)”; false 表示“允许剔废”
	// =========================

	if (!isbad)
	{
		// 良品：累计连续良品；并清空连续剔废计数（因为已经不连续了）
		tifeiCount = 0;
		++liangpinCount;

		// 只有在“锁定状态”下，连续良品达到门槛才解锁
		if (needGoodReset && (liangpinCount >= setConfig.liangpinjishu))
		{
			needGoodReset = false;	// 解锁：重新允许剔废窗口打开
			tifeiCount = 0;			// 新一轮剔废窗口，计数从0开始
		}
		return;
	}

	// 次品：连续良品被打断
	liangpinCount = 0;

	// 只要有坏的就报警
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (mainWindowConfig.isbaojing)
	{
		auto isSuccess = zmotion->SetIOOut(2, ControlLines::baojingOut, true, static_cast<int>(setConfig.baojingchixushijian));
		isSuccess = zmotion->SetIOOut(3, ControlLines::hongdengOut, true, static_cast<int>(setConfig.baojingchixushijian));
		isSuccess = zmotion->SetIOOut(4, ControlLines::lvdengOut, false, static_cast<int>(setConfig.baojingchixushijian));

		int defectClassId = Modules::getInstance().runtimeInfoModule.lastDefectClassId.load();
		qDebug() << "defectClassId:" << defectClassId;
		rw::rqw::WarningInfo WarningInfo;
		WarningInfo.warningId = defectCount;
		WarningInfo.message = QString(QTime::currentTime().toString() + "  " + osoFileUtilityFunc::defectClassIdToText(defectClassId));
		qDebug() << "osoFileUtilityFunc::defectClassIdToText(defectClassId):" << osoFileUtilityFunc::defectClassIdToText(defectClassId);
		WarningInfo.type = rw::rqw::WarningType::Warning;
		QMetaObject::invokeMethod(this,
			[this, WarningInfo]() {
				MDOFoodBags::addWarning(WarningInfo);
			});
		++defectCount;
	}

	// 如果处于锁定状态：次品不剔废，直接返回
	if (needGoodReset)
	{
		return;
	}

	// 允许剔废状态：次品 => 执行剔废，并累计“连续剔废次数”
	priorityQueue1->push(defectLoc);

	++tifeiCount;

	// 当连续剔废次数达到门槛：上锁（下一张开始不再剔废）
	if (tifeiCount >= setConfig.tifeijishu)
	{
		needGoodReset = true;	// 上锁
		liangpinCount = 0;		// 开始准备累计连续良品用于解锁
	}
}

void ImageProcessor::save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image, int imageIndex)
{

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

	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;

	// 不对左右限位直线进行像素当量计算 —— 直接把传入的值作为绘制位置（逻辑坐标）
	// 若传入的就是像素值，则线会按像素绘制；若不是，请确保调用处传入期望的坐标系。
	int leftX = static_cast<int>(std::round(leftLimit));
	int rightX = static_cast<int>(std::round(rightLimit));

	// 绘制左右限位线 (垂直线)
	painter.drawLine(leftX, 0, leftX, imageHeight);
	painter.drawLine(rightX, 0, rightX, imageHeight);

	// 准备字体与度量
	QFont font = painter.font();
	font.setPointSize(25);
	font.setBold(true);
	painter.setFont(font);
	QFontMetrics metrics(font);

	// 绘制左限位标签（显示逻辑值，并同时显示像素当量）
	// 计算像素当量用于显示：pixel = logical * xiangSuDangLiang
	int leftPixel = static_cast<int>(std::round(leftLimit * setConfig.xiangSuDangLiang));
	QString leftText = QString("左限位: %1").arg(leftPixel);
	QRect leftTextRect = metrics.boundingRect(leftText);
	leftTextRect.moveTo(leftX + 10, 50);
	leftTextRect.adjust(-5, -5, 5, 5);
	painter.fillRect(leftTextRect, QColor(0, 0, 0, 180));
	painter.setPen(Qt::white);
	painter.drawText(leftTextRect, Qt::AlignCenter, leftText);

	// 绘制右限位标签
	int rightPixel = static_cast<int>(std::round(rightLimit * setConfig.xiangSuDangLiang));
	QString rightText = QString("右限位: %1").arg(rightPixel);
	QRect rightTextRect = metrics.boundingRect(rightText);
	rightTextRect.moveTo(rightX - rightTextRect.width() - 10, 50);
	rightTextRect.adjust(-5, -5, 5, 5);
	painter.fillRect(rightTextRect, QColor(0, 0, 0, 180));
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
	int defectWidth = x2 - x1;
	int defectHeight = y2 - y1;

	// 获取图像尺寸
	int imageWidth = sourceImage.width();
	int imageHeight = sourceImage.height();

	// 计算区域中心点
	int centerX = (x1 + x2) / 2;
	int centerY = (y1 + y2) / 2;

	// 确定正方形的边长(取宽高中的较大值,并确保不小于minSize)
	int sideLength = defectWidth > defectHeight ? defectWidth : defectHeight;
	sideLength = sideLength > minSize ? sideLength : minSize;

	// 以中心点为基准计算正方形的坐标(注意：这里会覆盖 x1/y1/x2/y2，用于裁剪区域)
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
	int cropWidth = x2 - x1;
	int cropHeight = y2 - y1;

	// 确保宽高为正数
	if (cropWidth <= 0 || cropHeight <= 0)
	{
		return QImage();
	}

	// 提取图像区域
	QRect extractRect(x1, y1, cropWidth, cropHeight);
	QImage extractedImage = sourceImage.copy(extractRect);
	if (extractedImage.isNull())
	{
		return QImage();
	}

	// ============================
	// 在裁剪图上绘制“原始瑕疵区域”矩形 + 不遮挡文字(字号自适应)
	// ============================
	QRect defectRectGlobal(static_cast<int>(result.C1),
		static_cast<int>(result.R1),
		static_cast<int>(result.C2 - result.C1),
		static_cast<int>(result.R2 - result.R1));
	defectRectGlobal = defectRectGlobal.normalized();

	QRect defectRectLocal(defectRectGlobal.x() - extractRect.x(),
		defectRectGlobal.y() - extractRect.y(),
		defectRectGlobal.width(),
		defectRectGlobal.height());

	defectRectLocal = defectRectLocal.intersected(QRect(0, 0, extractedImage.width(), extractedImage.height()));
	const double scale = 1.5;

	int cx = defectRectLocal.x() + defectRectLocal.width() / 2;
	int cy = defectRectLocal.y() + defectRectLocal.height() / 2;

	int newW = static_cast<int>(std::round(defectRectLocal.width() * scale));
	int newH = static_cast<int>(std::round(defectRectLocal.height() * scale));

	int nx1 = cx - newW / 2;
	int ny1 = cy - newH / 2;
	int nx2 = nx1 + newW;
	int ny2 = ny1 + newH;

	// 边界裁剪（不使用 std::min/std::max）
	nx1 = (nx1 < 0) ? 0 : nx1;
	ny1 = (ny1 < 0) ? 0 : ny1;

	const int maxX = extractedImage.width() - 1;
	const int maxY = extractedImage.height() - 1;

	nx2 = (nx2 > maxX) ? maxX : nx2;
	ny2 = (ny2 > maxY) ? maxY : ny2;

	int nw = nx2 - nx1;
	int nh = ny2 - ny1;

	if (nw > 0 && nh > 0)
	{
		defectRectLocal = QRect(nx1, ny1, nw, nh);
	}
	if (!defectRectLocal.isEmpty())
	{
		// 文字内容（包含类型）
		QString defectTypeText;
		if (result.classid == 0)
		{
			defectTypeText = "蚊虫";
		}
		else if (result.classid == 1)
		{
			defectTypeText = "毛发";
		}
		else
		{
			defectTypeText = "未知";
		}

		QString infoText = QString("%1  A:%2  G:%3")
			.arg(defectTypeText)
			.arg(result.Area, 0, 'f', 2)
			.arg(result.MeanThreshold, 0, 'f', 2);

		// 字号自适应：按“短边”比例缩放，并夹紧范围
		int minSide = extractedImage.width() < extractedImage.height() ? extractedImage.width() : extractedImage.height();
		int fontSize = static_cast<int>(std::round(minSide * 0.10)); // 10%短边
		if (fontSize < 10) fontSize = 10;
		if (fontSize > 28) fontSize = 28;

		QFont font;
		font.setPointSize(fontSize);
		font.setBold(true);

		// 先用 metrics 计算文字区域尺寸
		QFontMetrics metrics(font);
		QRect textRect = metrics.boundingRect(infoText);
		const int pad = 6;
		textRect.adjust(-pad, -pad, pad, pad);

		// 先在原图上画框
	/*	{
			

			
			QPainter painter(&extractedImage);
			painter.setRenderHint(QPainter::Antialiasing, true);

			QPen pen(Qt::red);
			pen.setWidth(2);
			painter.setPen(pen);
			painter.setBrush(Qt::NoBrush);
			painter.drawRect(defectRectLocal);

			painter.end();
		}*/

		const QRect imgRect(0, 0, extractedImage.width(), extractedImage.height());

		// 候选位置：上方(优先) -> 下方 -> 右侧 -> 左侧
		QRect candidateTop(defectRectLocal.center().x() - textRect.width() / 2,
			defectRectLocal.top() - textRect.height() - 4,
			textRect.width(),
			textRect.height());

		QRect candidateBottom(defectRectLocal.center().x() - textRect.width() / 2,
			defectRectLocal.bottom() + 4,
			textRect.width(),
			textRect.height());

		QRect candidateRight(defectRectLocal.right() + 4,
			defectRectLocal.center().y() - textRect.height() / 2,
			textRect.width(),
			textRect.height());

		QRect candidateLeft(defectRectLocal.left() - textRect.width() - 4,
			defectRectLocal.center().y() - textRect.height() / 2,
			textRect.width(),
			textRect.height());

		auto fits = [&](const QRect& r) {
			return imgRect.contains(r);
			};

		QRect finalRect;
		bool needExpandCanvas = false;

		if (fits(candidateTop)) {
			finalRect = candidateTop;
		}
		else if (fits(candidateBottom)) {
			finalRect = candidateBottom;
		}
		else if (fits(candidateRight)) {
			finalRect = candidateRight;
		}
		else if (fits(candidateLeft)) {
			finalRect = candidateLeft;
		}
		else {
			needExpandCanvas = true;
		}

		if (!needExpandCanvas)
		{
			QPainter painter(&extractedImage);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.setFont(font);

			painter.fillRect(finalRect, QColor(0, 0, 0, 140));
			painter.setPen(Qt::white);
			painter.drawText(finalRect, Qt::AlignCenter, infoText);

			painter.end();

			return extractedImage;
		}

		// ============================
		// 四周都放不下：扩展画布，确保文字可见且不遮挡瑕疵
		// 默认优先扩展顶部；如仍不够则扩展底部
		// ============================
		const int gap = 4;
		int extraTop = textRect.height() + gap * 2;
		int extraBottom = 0;
		int extraLeft = 0;
		int extraRight = 0;

		// 如果文字宽度比图还宽，则左右也扩一点，保证文字完整显示
		if (textRect.width() + gap * 2 > extractedImage.width())
		{
			int needW = (textRect.width() + gap * 2) - extractedImage.width();
			extraLeft = needW / 2;
			extraRight = needW - extraLeft;
		}

		QImage expanded(extractedImage.width() + extraLeft + extraRight,
			extractedImage.height() + extraTop + extraBottom,
			extractedImage.format());
		expanded.fill(Qt::black);

		{
			QPainter painter(&expanded);
			painter.setRenderHint(QPainter::Antialiasing, true);

			// 把原图贴到新画布
			painter.drawImage(extraLeft, extraTop, extractedImage);

			// 文字放到新增顶部区域，居中
			QRect topBandRect(extraLeft + (extractedImage.width() - textRect.width()) / 2,
				(extraTop - textRect.height()) / 2,
				textRect.width(),
				textRect.height());

			painter.setFont(font);
			painter.fillRect(topBandRect, QColor(0, 0, 0, 140));
			painter.setPen(Qt::white);
			painter.drawText(topBandRect, Qt::AlignCenter, infoText);

			painter.end();
		}

		return expanded;
	}

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
