#include "CameraAndCardStateThread.h"

#include "Modules.hpp"

size_t CameraAndCardStateThreadMDOFoodBags::runtimeCounts=0;

CameraAndCardStateThreadMDOFoodBags::CameraAndCardStateThreadMDOFoodBags(QObject* parent)
	: QThread(parent), running(false), _dlgProductSet(Modules::getInstance().configManagerModule.setConfig){
}

CameraAndCardStateThreadMDOFoodBags::~CameraAndCardStateThreadMDOFoodBags()
{
	stopThread();
	wait();
}

void CameraAndCardStateThreadMDOFoodBags::startThread()
{
	running = true;
	if (!isRunning()) {
		start();
	}
}

void CameraAndCardStateThreadMDOFoodBags::stopThread()
{
	running = false;
}

void CameraAndCardStateThreadMDOFoodBags::run()
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

void CameraAndCardStateThreadMDOFoodBags::check_cameraState()
{
	check_cameraState1();
}

void CameraAndCardStateThreadMDOFoodBags::check_cameraState1()
{
	static bool isUpdateState = false;

	auto& camera1 = Modules::getInstance().cameraModule.camera1;

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

void CameraAndCardStateThreadMDOFoodBags::check_cardState()
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
