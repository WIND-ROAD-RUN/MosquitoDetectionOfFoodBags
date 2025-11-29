#pragma once


#include <opencv2/opencv.hpp>
#include <vector>
#include <QPixmap>

#include "rqw_CameraObjectCore.hpp"
#include "rqw_ImageSaveEngine.h"

// 图片信息
struct MatInfo {
	cv::Mat image;	// 图片内容
	float location;	// 记录拍照瞬间的时间点
	size_t index;	// 拍照的相机的下标
};


class ImageProcessor : public QThread
{
	Q_OBJECT

public:
	ImageProcessor(QQueue<MatInfo>& queue,
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



	void halconPRocess(cv::Mat image, QVector< double>& R1, QVector< double>& C1, QVector< double>& R2, QVector< double>& C2, QVector< double>& Area);
	// 在图像上绘制矩形
	void drawRectanglesOnImage(QImage& image,
		const QVector<double>& R1,
		const QVector<double>& C1,
		const QVector<double>& R2,
		const QVector<double>& C2,
		const QVector<double>& Areas,
		const QColor& color = Qt::red,
		int penWidth = 2);
	bool checkDefectAndDrawOnImage(QImage& image, const QVector<double>& R1, const QVector<double>& C1,
	                               const QVector<double>& R2, const QVector<double>& C2, const QVector<double>& Areas,
	                               double minArea, double allMinArea);
	// 在图像上绘制单个矩形
	void drawSingleRectangleOnImage(QImage& image,
		double R1,
		double C1,
		double R2,
		double C2,
		double area,
		const QColor& color = Qt::red,
		int penWidth = 2);



	void drawLimitLines(QImage& image,
	                    double leftLimit,
	                    double rightLimit,
	                    const QColor& color = Qt::yellow,
	                    int penWidth = 2);

	// 存图
	void save_image(rw::rqw::ImageInfo& imageInfo, const QImage& image);
	void save_image_work(rw::rqw::ImageInfo& imageInfo, const QImage& image);

signals:
	void imageNGReady(QPixmap image, size_t index, bool isbad);

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
};


class ImageProcessingModule : public QObject {
	Q_OBJECT
public:
	QString modelEnginePath;
public:
	// 初始化图像处理模块
	void BuildModule();
public:
	ImageProcessingModule(int numConsumers, QObject* parent = nullptr);

	~ImageProcessingModule();

public slots:
	// 相机回调函数
	void onFrameCaptured(rw::rqw::MatInfo matInfo, size_t index, float loc);

signals:
	void imageNGReady(QPixmap image, size_t index, bool isbad);

public:
	std::vector<ImageProcessor*> getProcessors() const {
		return _processors;
	}

private:
	QQueue<MatInfo> _queue;
	QMutex _mutex;
	QWaitCondition _condition;
	std::vector<ImageProcessor*> _processors;
	int _numConsumers;
public:
	size_t index;
};








