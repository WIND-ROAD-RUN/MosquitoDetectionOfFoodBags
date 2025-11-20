#pragma once

#
#include <opencv2/opencv.hpp>
#include <vector>
#include <QPixmap>
#include <imgPro_ImageProcess.hpp>

#include "rqw_CameraObjectCore.hpp"
#include "rqw_ImageSaveEngine.h"

// 图片信息
struct MatInfo {
	cv::Mat image;	// 图片内容
	float location;	// 记录拍照瞬间的时间点
	size_t index;	// 拍照的相机的下标
};


class ImageProcessorWetPapers : public QThread
{
	Q_OBJECT

public:
	ImageProcessorWetPapers(QQueue<MatInfo>& queue,
		QMutex& mutex,
		QWaitCondition& condition,
		int workIndex,
		QObject* parent = nullptr);

protected:
	void run() override;

private:
	void run_debug(MatInfo& frame);				// 不开剔废时候的调试模式
private:
	void run_OpenRemoveFunc(MatInfo& frame);	// 开启剔废功能时的处理模式

	void run_OpenRemoveFunc_emitErrorInfo(bool isbad) const;

	// 存图
	void save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image);
	void save_image_work(rw::rqw::ImageInfo& imageInfo, const QImage& image);

signals:
	void imageNGReady(QPixmap image, size_t index, bool isbad);

private:
	std::unique_ptr<rw::imgPro::ImageProcess> _imgProcess;
public:
	// 构建模型引擎
	void buildSegModelEngine(const QString& enginePath);		// Segmentation 模型

private:
	// 判断是否有缺陷
	bool _isbad{ false };

private:
	QQueue<MatInfo>& _queue;
	QMutex& _mutex;
	QWaitCondition& _condition;
	int _workIndex;
public:
	int imageProcessingModuleIndex;

	std::vector<int> DefectBox{};
};


class ImageProcessingModuleWetPapers : public QObject {
	Q_OBJECT
public:
	QString modelEnginePath;
public:
	// 初始化图像处理模块
	void BuildModule();
public:
	ImageProcessingModuleWetPapers(int numConsumers, QObject* parent = nullptr);

	~ImageProcessingModuleWetPapers();

public slots:
	// 相机回调函数
	void onFrameCaptured(rw::rqw::MatInfo matInfo, size_t index, float loc);

signals:
	void imageNGReady(QPixmap image, size_t index, bool isbad);
	void shibiekuangChanged();
	void wenziChanged();
	void changeToDebugMode();
	void changeToRemoveMode();
	void paramMapsChanged();

public:
	std::vector<ImageProcessorWetPapers*> getProcessors() const {
		return _processors;
	}

private:
	QQueue<MatInfo> _queue;
	QMutex _mutex;
	QWaitCondition _condition;
	std::vector<ImageProcessorWetPapers*> _processors;
	int _numConsumers;
public:
	size_t index;
};


