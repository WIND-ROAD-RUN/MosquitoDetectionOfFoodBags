#include "ImageProcessorModule.hpp"
#include "imet_Utility.cuh"
#include "ime_ModelEngineFactory.h"
#include "Modules.hpp"
#include "Utilty.hpp"

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
	auto& imgPro = *_imgProcess;
	imgPro(frame.image);
	auto maskImg = imgPro.getMaskImg(frame.image);
	auto defectResult = imgPro.getDefectResultInfo();


	emit imageNGReady(QPixmap::fromImage(maskImg), frame.index, defectResult.isBad);
}

void ImageProcessor::run_OpenRemoveFunc(MatInfo& frame)
{
	DefectBox.clear();
	_isbad = false;
	auto& imgPro = *_imgProcess;
	imgPro(frame.image);
	auto maskImg = imgPro.getMaskImg(frame.image);
	auto defectResult = imgPro.getDefectResultInfo();
	auto imageRealLocation = frame.location;

	auto& context = _imgProcess->context();

	if (context.customFields.find("DefectBox") != context.customFields.end())
	{
		DefectBox = std::any_cast<std::vector<int>>(context.customFields.at("DefectBox"));
	}

	for (auto& item : DefectBox)
	{
		item = imageRealLocation + item;
	}
	run_OpenRemoveFunc_emitErrorInfo(defectResult.isBad);

	emit imageNGReady(QPixmap::fromImage(maskImg), frame.index, defectResult.isBad);

	if (defectResult.isBad)
	{
		_isbad = true;
	}
	rw::rqw::ImageInfo imageInfo(rw::rqw::cvMatToQImage(frame.image));

	save_image(imageInfo, maskImg);
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

	if (isbad)
	{
		for (const auto& item : DefectBox)
		{
			if (1 == imageProcessingModuleIndex)
			{
				priorityQueue1->push(item);
			}
		}
	}
}

void ImageProcessor::save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& isTakePictures = Modules::getInstance().runtimeInfoModule.isTakePictures;

	if (!isTakePictures)
	{
		return;
	}

	if ((imageProcessingModuleIndex == 1 && setConfig.takeWork1Pictures) || (imageProcessingModuleIndex == 2 && setConfig.takeWork2Pictures))
	{
		save_image_work(imageInfo, image);
	}
}

void ImageProcessor::save_image_work(rw::rqw::ImageInfo& imageInfo, const QImage& image)
{
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	if (_isbad) {
		if (setConfig.saveNGImg)
		{
			if (1 == imageProcessingModuleIndex)
			{
				imageInfo.classify = "NG1";
				imageSaveEngine->pushImage(imageInfo);
			}
			else if (2 == imageProcessingModuleIndex)
			{
				imageInfo.classify = "NG2";
				imageSaveEngine->pushImage(imageInfo);
			}
		}
		if (setConfig.saveMaskImg)
		{
			rw::rqw::ImageInfo mask(image);
			if (1 == imageProcessingModuleIndex)
			{
				mask.classify = "Mask1";
				imageSaveEngine->pushImage(mask);
			}
			else if (2 == imageProcessingModuleIndex)
			{
				mask.classify = "Mask2";
				imageSaveEngine->pushImage(mask);
			}
		}
	}
	else {
		if (setConfig.saveOKImg)
		{
			if (1 == imageProcessingModuleIndex)
			{
				imageInfo.classify = "OK1";
				imageSaveEngine->pushImage(imageInfo);
			}
			else if (2 == imageProcessingModuleIndex)
			{
				imageInfo.classify = "OK2";
				imageSaveEngine->pushImage(imageInfo);
			}
		}
	}
}

void ImageProcessor::buildSegModelEngine(const QString& enginePath)
{
	rw::ModelEngineConfig modelEngineConfig;
	modelEngineConfig.conf_threshold = 0.1f;
	modelEngineConfig.nms_threshold = 0.1f;
	modelEngineConfig.imagePretreatmentPolicy = rw::ImagePretreatmentPolicy::LetterBox;
	modelEngineConfig.letterBoxColor = cv::Scalar(114, 114, 114);
	modelEngineConfig.modelPath = enginePath.toStdString();
	auto engine = rw::ModelEngineFactory::createModelEngine(modelEngineConfig, rw::ModelType::Yolov11_Seg_Mask, rw::ModelEngineDeployType::TensorRT);

	_imgProcess = std::make_unique<rw::imgPro::ImageProcess>(engine);
	_imgProcess->context() = Modules::getInstance().imgProModule.imageProcessContext_Main;
	_imgProcess->context().customFields["ImgProcessIndex"] = static_cast<int>(imageProcessingModuleIndex);
	_imgProcess->context().customFields["stationIdx"] = static_cast<int>(_workIndex);
}

void ImageProcessingModule::BuildModule()
{
	for (int i = 0; i < _numConsumers; ++i) {
		static size_t workIndexCount = 0;
		ImageProcessor* processor = new ImageProcessor(_queue, _mutex, _condition, workIndexCount, this);
		workIndexCount++;
		processor->imageProcessingModuleIndex = index;
		processor->buildSegModelEngine(modelEnginePath);
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


	QMutexLocker locker(&_mutex);
	MatInfo mat;
	mat.image = matInfo.mat;
	mat.index = index;

	mat.location = loc;

	_queue.enqueue(mat);
	_condition.wakeOne();
}
