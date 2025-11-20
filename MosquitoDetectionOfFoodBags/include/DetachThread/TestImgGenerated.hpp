#pragma once
#include <qtmetamacros.h>
#include "rqw_CameraObject.hpp"
#include <QThread>
#include <QObject>

class DetachTestImgThread
	: public QThread
{
	Q_OBJECT
public:
	std::atomic_bool isProcessing{ false };
	std::atomic_bool isProcessFinish{ false };
private:
	rw::rqw::MatInfo matInfo1;
public:
	explicit DetachTestImgThread(QObject* parent = nullptr);

	~DetachTestImgThread() override;

	void startThread();

	void stopThread();
private:
	std::atomic<bool> _running;
signals:
	void frameCaptured(rw::rqw::MatInfo matInfo, size_t index, float location);
protected:
	void run() override;
};
