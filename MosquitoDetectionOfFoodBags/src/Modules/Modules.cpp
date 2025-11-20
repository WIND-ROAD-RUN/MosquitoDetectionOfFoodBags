#include "Modules.hpp"

#include <QMessageBox>

#include "DlgProductScore.h"
#include "DlgProductSet.h"
#include "MDOFoodBags.h"

Modules::Modules()
{

}

Modules::~Modules()
{

}

bool Modules::build()
{
	// 读取配置
	auto configManagerBuild = configManagerModule.build();

	// 构建相机
	auto cameraBuild = cameraModule.build();

	// 构建显示信息模块
	auto runtimeInfoModuleBuild = runtimeInfoModule.build();

	// 构建图像处理模块
	auto imgProModuleBuild = imgProModule.build();

	// 构建UI模块
	uiModule.build();

	// 构建重连模块
	reconnectModule.build();

	// 构建消除模块
	auto eliminateModuleBuild = eliminateModule.build();

	// 构建图像保存模块
	imgSaveModule.build();

	// 构建运动控制模块
	auto motionControllerModuleBuild = motionControllerModule.build();

	return true;
}

void Modules::destroy()
{
	runtimeInfoModule.destroy();
	imgProModule.destroy();
	cameraModule.destroy();
	configManagerModule.destroy();
	uiModule.destroy();
	reconnectModule.destroy();
	eliminateModule.destroy();
	imgSaveModule.destroy();
	motionControllerModule.destroy();
}

void Modules::start()
{
	uiModule.start();
	configManagerModule.start();
	motionControllerModule.start();
	runtimeInfoModule.start();
	imgSaveModule.start();
	eliminateModule.start();
	imgProModule.start();
	cameraModule.start();
	reconnectModule.start();
}

void Modules::stop()
{
	reconnectModule.stop();
	cameraModule.stop();
	imgProModule.stop();
	eliminateModule.stop();
	imgSaveModule.stop();
	runtimeInfoModule.stop();
	motionControllerModule.stop();
	configManagerModule.stop();
	uiModule.stop();
}

void Modules::connect()
{

#pragma region connect camera and imgProModule

	QObject::connect(&cameraModule, &CameraModule::frameCaptured1,
		imgProModule.imageProcessingModule1.get(), &ImageProcessingModuleWetPapers::onFrameCaptured, Qt::DirectConnection);
#pragma endregion

#pragma region connect UIModule and imgProModule
	QObject::connect(imgProModule.imageProcessingModule1.get(), &ImageProcessingModuleWetPapers::imageNGReady, uiModule._wetPapers, &MDOFoodBags::onCameraDisplay);
	QObject::connect(imgProModule.imageProcessingModule2.get(), &ImageProcessingModuleWetPapers::imageNGReady, uiModule._wetPapers, &MDOFoodBags::onCameraDisplay);
	QObject::connect(uiModule._wetPapers, &MDOFoodBags::shibiekuangChanged, &imgProModule, &ImgProModule::onUpdateImgProContext);
	QObject::connect(uiModule._wetPapers, &MDOFoodBags::shibiekuangChanged, &imgProModule, &ImgProModule::onUpdateImgProContext);
	QObject::connect(uiModule._dlgProductScore, &DlgProductScore::scoreFormClosed, &imgProModule, &ImgProModule::onUpdateImgProContext);

#pragma endregion

#pragma region connect UIModule and ReconnectModule
	// 更新UI界面
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadMDOFoodBags::updateCameraLabelState,
		uiModule._wetPapers, &MDOFoodBags::updateCameraLabelState, Qt::QueuedConnection);
	// 相机重连
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadMDOFoodBags::buildCamera,
		&cameraModule, &CameraModule::onBuildCamera, Qt::QueuedConnection);
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadMDOFoodBags::startMonitor,
		&cameraModule, &CameraModule::onStartCamera, Qt::QueuedConnection);
	// 相机销毁
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadMDOFoodBags::destroyCamera,
		&cameraModule, &CameraModule::onDestroyCamera, Qt::QueuedConnection);

	// 运动控制器重连
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadMDOFoodBags::buildZmotion,
		&motionControllerModule, &MotionControllerModule::onBuildZmotion, Qt::QueuedConnection);
	// 运动控制器销毁
	QObject::connect(reconnectModule.monitorCameraAndCardStateThread.get(), &CameraAndCardStateThreadMDOFoodBags::destroyZmotion,
		&motionControllerModule, &MotionControllerModule::onDestroyZmotion, Qt::QueuedConnection);
#pragma endregion

#pragma region connect UIModule and MotionControllerModule
	QObject::connect(motionControllerModule.monitorMotionIoStateThread.get(), &rw::rqw::MonitorZMotionIOStateThread::DIState,
		uiModule._dlgProductSet, &DlgProductSet::monitorInPutSignal, Qt::QueuedConnection);
	QObject::connect(motionControllerModule.monitorMotionIoStateThread.get(), &rw::rqw::MonitorZMotionIOStateThread::DOState,
		uiModule._dlgProductSet, &DlgProductSet::monitorOutPutSignal, Qt::QueuedConnection);
#pragma endregion

#pragma region connect UIModule and RuntimeInfoModule
	QObject::connect(runtimeInfoModule.detachUtiltyThread.get(), &DetachUtiltyThread::updateStatisticalInfo,
		uiModule._wetPapers, &MDOFoodBags::onUpdateStatisticalInfoUI, Qt::QueuedConnection);
#pragma endregion

}

bool Modules::check()
{
#pragma region check single instance
	if (!rw::rqw::RunEnvCheck::isSingleInstance("WetPapers.exe"))
	{
		QMessageBox::warning(nullptr, "错误", "已经有程序在运行，请勿多次打开");
		return false;
	}
#pragma endregion

#pragma region check run env
	if (rw::rqw::RunEnvCheck::isProcessRunning("MVS.exe"))
	{
		QMessageBox::warning(nullptr, "错误", "检测到海康威视软件正在运行，请先关闭后再启动本程序。");
		return false;
	}

	if (rw::rqw::RunEnvCheck::isProcessRunning("BasedCam3.exe"))
	{
		QMessageBox::warning(nullptr, "错误", "检测到度申相机平台软件正在运行，请先关闭后再启动本程序。");
		return false;
	}
#pragma endregion

#pragma region check directory exist
	EnsureDirectoryExists(globalPath.projectHome);
	EnsureDirectoryExists(globalPath.configRootPath);
	EnsureDirectoryExists(globalPath.modelRootPath);
	EnsureDirectoryExists(globalPath.imageSaveRootPath);
#pragma endregion

#pragma region check model exist
	if (!rw::rqw::RunEnvCheck::isFileExist(globalPath.modelPath))
	{
		QMessageBox::warning(nullptr, "错误", "模型文件缺失");
		return false;
	}
#pragma endregion

#pragma region check config format and exist
	rw::oso::StorageContext storageContext(rw::oso::StorageType::Xml);

	checkFileExistAndFormat<cdm::MainWindowsConfig>(globalPath.MainWindwsConfigPath, storageContext);
	checkFileExistAndFormat<cdm::SetConfig>(globalPath.DlgProductSetConfigPath, storageContext);
	checkFileExistAndFormat<cdm::ScoreConfig>(globalPath.DlgProductScoreConfigPath, storageContext);
#pragma endregion

	return true;
}

bool Modules::EnsureDirectoryExists(const QString& dirPath)
{
	QDir dir(dirPath);
	if (!dir.exists()) {
		return dir.mkpath(".");
	}
	return true;
}
