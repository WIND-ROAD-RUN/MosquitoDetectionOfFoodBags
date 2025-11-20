#include "CameraAndCardStateThread.h"

#include "GlobalStruct.hpp"
#include "Modules.hpp"
#include "rqw_CameraObjectZMotion.hpp"

size_t CameraAndCardStateThreadWetPapers::runtimeCounts=0;

CameraAndCardStateThreadWetPapers::CameraAndCardStateThreadWetPapers(QObject* parent)
	: QThread(parent), running(false), _dlgProductSet(Modules::getInstance().configManagerModule.setConfig){
}

CameraAndCardStateThreadWetPapers::~CameraAndCardStateThreadWetPapers()
{
	stopThread();
	wait();
}

void CameraAndCardStateThreadWetPapers::startThread()
{
	running = true;
	if (!isRunning()) {
		start();
	}
}

void CameraAndCardStateThreadWetPapers::stopThread()
{
	running = false;
}

void CameraAndCardStateThreadWetPapers::run()
{
	while (running) {
		QThread::msleep(2000);
		if (_dlgProductSet.yundongkongzhiqichonglian)
		{
			check_cardState();
		}

		check_cameraState();

		runtimeCounts++;
		if (runtimeCounts == 4) {
			runtimeCounts = 0;
		}
	}
}

void CameraAndCardStateThreadWetPapers::check_cameraState()
{
	check_cameraState1();
	if (_dlgProductSet.qiyongerxiangji)
	{
		check_cameraState2();
	}
}

void CameraAndCardStateThreadWetPapers::check_cameraState1()
{
	static bool isUpdateState = false;

	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& globalThread = GlobalThread::getInstance();

	if (runtimeCounts != 0) {
		return;
	}
	if (camera1) {
		if (camera1->getConnectState()) {
			if (!isUpdateState) {
				emit updateCameraLabelState(1, true);
				isUpdateState = true;
			}
		}
		else {
			emit destroyCamera(1);
			emit updateCameraLabelState(1, false);
			isUpdateState = false;
		}
	}
	else {
		emit buildCamera(1);
		emit startMonitor(1);
		emit updateCameraLabelState(1, false);
		isUpdateState = false;
	}
}

void CameraAndCardStateThreadWetPapers::check_cameraState2()
{
	static bool isUpdateState = false;

	auto& camera2 = Modules::getInstance().cameraModule.camera2;

	if (runtimeCounts != 1) {
		return;
	}

	if (camera2) {
		if (camera2->getConnectState()) {
			if (!isUpdateState) {
				emit updateCameraLabelState(2, true);
				isUpdateState = true;
			}
		}
		else {
			emit destroyCamera(2);
			emit updateCameraLabelState(2, false);
			isUpdateState = false;
		}
	}
	else {
		emit buildCamera(2);
		emit startMonitor(2);
		emit updateCameraLabelState(2, false);
		isUpdateState = false;
	}
}

void CameraAndCardStateThreadWetPapers::check_cardState()
{
	static bool isUpdateState = false;

	auto& zMotion = Modules::getInstance().motionControllerModule.zmotion;

	if (runtimeCounts != 3) {
		return;
	}

	if (zMotion)
	{
		if (zMotion->getConnectState())
		{
			if (!isUpdateState) {
				emit updateCameraLabelState(0, true);
				isUpdateState = true;
			}
		}
		else {
			emit buildZmotion();
			emit updateCameraLabelState(0, false);
			isUpdateState = false;
		}
	}
	else {
		emit buildZmotion();
		emit updateCameraLabelState(0, false);
		isUpdateState = false;
	}
}
