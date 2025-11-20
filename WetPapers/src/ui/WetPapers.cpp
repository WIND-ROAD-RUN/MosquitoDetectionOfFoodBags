#include "ui_WetPapers.h"

#include "WetPapers.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>

#include "DlgCloseForm.h"
#include "DlgProductScore.h"
#include "DlgProductSet.h"
#include "GlobalStruct.hpp"
#include "Modules.hpp"
#include "NumberKeyboard.h"
#include"Utilty.hpp"

WetPapers::WetPapers(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::PunchPressClass())
{
	ui->setupUi(this);

	initializeComponents();

}

WetPapers::~WetPapers()
{
	destroyComponents();
	Modules::getInstance().stop();
	Modules::getInstance().destroy();
	delete ui;
}

void WetPapers::build_ui()
{
	build_WetPapersData();

	//弃用
	/*ui->label_produceTotal->setVisible(false);
	ui->label_produceTotalValue->setVisible(false);
	ui->label_wasteProducts->setVisible(false);
	ui->label_wasteProductsValue->setVisible(false);
	ui->label_productionYield->setVisible(false);
	ui->label_productionYieldValue->setVisible(false);*/
}

void WetPapers::build_connect()
{
	connect(ui->pbtn_exit, &QPushButton::clicked, this, &WetPapers::pbtn_exit_clicked);
	connect(ui->pbtn_set, &QPushButton::clicked, this, &WetPapers::pbtn_set_clicked);
	connect(ui->pbtn_score, &QPushButton::clicked, this, &WetPapers::pbtn_score_clicked);
	connect(ui->rbtn_debug, &QRadioButton::toggled, this, &WetPapers::rbtn_debug_checked);
	connect(ui->rbtn_takePicture, &QRadioButton::toggled, this, &WetPapers::rbtn_takePicture_checked);
	connect(ui->rbtn_removeFunc, &QRadioButton::toggled, this, &WetPapers::rbtn_removeFunc_checked);
	connect(ui->ckb_shibiekuang, &QCheckBox::toggled, this, &WetPapers::ckb_shibiekuang_checked);
	connect(ui->ckb_wenzi, &QCheckBox::toggled, this, &WetPapers::ckb_wenzi_checked);
	connect(ui->pbtn_openSaveLocation, &QPushButton::clicked, this, &WetPapers::pbtn_openSaveLocation_clicked);

	// 连接显示标题
	QObject::connect(clickableTitle, &rw::rqw::ClickableLabel::clicked,
		this, &WetPapers::lb_title_clicked);
	connect(ui->pbtn_resetProduct, &QPushButton::clicked, this, &WetPapers::pbtn_resetProduct_clicked);
}

void WetPapers::build_WetPapersData()
{
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.wetPapersConfig;
	wetPapersConfig.isDebug = false;
	wetPapersConfig.isDefect = true;		// 默认开启剔废
	rbtn_removeFunc_checked(true);
	wetPapersConfig.isshibiekuang = true;
	wetPapersConfig.iswenzi = true;

	/*ui->label_produceTotalValue->setText(QString::number(wetPapersConfig.totalProductionVolume));
	ui->label_wasteProductsValue->setText(QString::number(wetPapersConfig.totalDefectiveVolume));
	ui->label_productionYieldValue->setText(QString::number(wetPapersConfig.productionYield));*/
	ui->rbtn_takePicture->setChecked(wetPapersConfig.isSaveImg);
	ui->rbtn_removeFunc->setChecked(wetPapersConfig.isDefect);
	ui->ckb_shibiekuang->setChecked(wetPapersConfig.isshibiekuang);
	ui->ckb_wenzi->setChecked(wetPapersConfig.iswenzi);

	// release版本
#ifdef NDEBUG
	wetPapersConfig.isSaveImg = true;
	ui->rbtn_takePicture->setChecked(true);
	ui->rbtn_debug->setVisible(false);
#endif


	Modules::getInstance().runtimeInfoModule.isTakePictures = wetPapersConfig.isSaveImg;

	// 初始化图像查看器
	_picturesViewer = new PictureViewerThumbnails(this);

	ini_clickableTitle();
}

void WetPapers::ini_clickableTitle()
{
	// 初始化标题label
	clickableTitle = new rw::rqw::ClickableLabel(this);
	auto layoutTitle = ui->groupBox_head->layout();
	layoutTitle->replaceWidget(ui->label_title, clickableTitle);
	delete ui->label_title;
	clickableTitle->setText("毛巾检测");
	clickableTitle->setStyleSheet("QLabel {font-size: 30px;font-weight: bold;color: rgb(255, 255, 255);padding: 5px 5px;border-bottom: 2px solid #cccccc;}");
}

void WetPapers::initializeComponents()
{
	build_ui();

	build_ImageEnlargedDisplay();

	getMotionStateAndUpdateUi();

	getCameraStateAndUpdateUi();

	build_connect();

#ifndef  BUILD_WITHOUT_HARDWARE
#endif


#ifdef BUILD_WITHOUT_HARDWARE
	build_DetachTestImgThread();
	start_DetachTestImgThread();
#endif


	//////////////////
	/*QThread* threadTifei = QThread::create([]() {

		rw::ModelEngineConfig config;
		config.conf_threshold = 0.1f;
		config.nms_threshold = 0.1f;
		config.imagePretreatmentPolicy = rw::ImagePretreatmentPolicy::LetterBox;
		config.letterBoxColor = cv::Scalar(114, 114, 114);
		config.modelPath = globalPath.modelPath.toStdString();
		auto engine = rw::ModelEngineFactory::createModelEngine(config, rw::ModelType::Yolov11_Seg_CudaAcc, rw::ModelEngineDeployType::TensorRT);

		cv::Mat colorImage(640, 640, CV_8UC3, cv::Scalar(0, 0, 0));
		while (true)
		{
			QThread::msleep(1);
			auto start = std::chrono::high_resolution_clock::now();
			engine->processImg(colorImage);
			auto end = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "processImg耗时:" << duration << std::endl;
		}
		});
	QObject::connect(threadTifei, &QThread::finished, threadTifei, &QThread::deleteLater);
	threadTifei->start();*/

}

void WetPapers::destroyComponents()
{
#ifdef BUILD_WITHOUT_HARDWARE
	stop_DetachTestImgThread();
	destroy_DetachTestImgThread();
#endif

	destroy_ImageEnlargedDisplay();
}

void WetPapers::getCameraStateAndUpdateUi()
{
	auto& cameraModules = Modules::getInstance().cameraModule;
	auto errors = cameraModules.getBuildResults();
	updateCameraLabelState(1, true);
	updateCameraLabelState(2, true);

	for (const auto& error : errors)
	{
		auto index = static_cast<int>(error);
		updateCameraLabelState(index, false);
	}
}

void WetPapers::getMotionStateAndUpdateUi()
{
	auto& motionControllerModule = Modules::getInstance().motionControllerModule;
	auto& isBuildZmotion = motionControllerModule.isConnectMotion;
	updateCameraLabelState(0, isBuildZmotion);
}

void WetPapers::build_ImageEnlargedDisplay()
{
	imgDis1 = new rw::rqw::ClickableLabel(this);
	imgDis1->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDis2 = new rw::rqw::ClickableLabel(this);
	imgDis2->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg1 = new rw::rqw::ClickableLabel(this);
	imgDisNg1->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg2 = new rw::rqw::ClickableLabel(this);
	imgDisNg2->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgDisplay_1, imgDis1);
	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgDisplay_3, imgDis2);

	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgDisplay_2, imgDisNg1);
	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgDisplay_4, imgDisNg2);

	delete ui->label_imgDisplay_1;
	delete ui->label_imgDisplay_3;
	delete ui->label_imgDisplay_2;
	delete ui->label_imgDisplay_4;

	QObject::connect(imgDis1, &rw::rqw::ClickableLabel::clicked
		, this, &WetPapers::imgDis1_clicked);
	QObject::connect(imgDis2, &rw::rqw::ClickableLabel::clicked
		, this, &WetPapers::imgDis2_clicked);
	QObject::connect(imgDisNg1, &rw::rqw::ClickableLabel::clicked
		, this, &WetPapers::imgDisNg1_clicked);
	QObject::connect(imgDisNg2, &rw::rqw::ClickableLabel::clicked
		, this, &WetPapers::imgDisNg2_clicked);

	_workStationTitleMap = {
		{0,"一号工位"},
		{1,"二号工位"},
		{2,"NG一号工位"},
		{3,"NG二号工位"}
	};

	_imageEnlargedDisplay = new ImageEnlargedDisplay(this);
	_imageEnlargedDisplay->setMonitorValue(&_isImageEnlargedDisplay);
	_imageEnlargedDisplay->setMonitorDisImgIndex(&_currentImageEnlargedDisplayIndex);
	_imageEnlargedDisplay->initWorkStationTitleMap(_workStationTitleMap);
	_imageEnlargedDisplay->setNum(2);
	_imageEnlargedDisplay->show();
	_imageEnlargedDisplay->close();
}

void WetPapers::destroy_ImageEnlargedDisplay()
{
	if (_imageEnlargedDisplay)
	{
		_imageEnlargedDisplay->close();
		delete _imageEnlargedDisplay;
		_imageEnlargedDisplay = nullptr;
	}
}

void WetPapers::updateCameraLabelState(int cameraIndex, bool state)
{
	switch (cameraIndex)
	{
	case 0:
		if (state) {
			ui->label_cardState->setText("连接成功");
			ui->label_cardState->setStyleSheet(QString("QLabel{color:rgb(0, 230, 0);} "));
		}
		else {
			ui->label_cardState->setText("连接失败");
			ui->label_cardState->setStyleSheet(QString("QLabel{color:rgb(230, 0, 0);} "));
		}
		break;
	case 1:
		if (state) {
			ui->label_camera1State->setText("连接成功");
			ui->label_camera1State->setStyleSheet(QString("QLabel{color:rgb(0, 230, 0);} "));
		}
		else {
			ui->label_camera1State->setText("连接失败");
			ui->label_camera1State->setStyleSheet(QString("QLabel{color:rgb(230, 0, 0);} "));
		}
		break;
	case 2:
		if (state) {
			ui->label_camera2State->setText("连接成功");
			ui->label_camera2State->setStyleSheet(QString("QLabel{color:rgb(0, 230, 0);} "));
		}
		else {
			ui->label_camera2State->setText("连接失败");
			ui->label_camera2State->setStyleSheet(QString("QLabel{color:rgb(230, 0, 0);} "));
		}
		break;
	default:
		break;
	}
}

void WetPapers::onUpdateStatisticalInfoUI()
{
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	/*ui->label_produceTotalValue->setText(QString::number(statisticalInfo.produceCount.load()));
	ui->label_wasteProductsValue->setText(QString::number(statisticalInfo.wasteCount.load()));
	ui->label_productionYieldValue->setText(QString::number(statisticalInfo.productionYield.load(), 'f', 2) + "%");*/
}

void WetPapers::onCameraNGDisplay(QPixmap image, size_t index, bool isbad)
{
	if (index == 1)
	{
		if (!_isImageEnlargedDisplay)
		{
			imgDis1->setPixmap(image.scaled(imgDis1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
			if (isbad)
			{
				imgDisNg1->setPixmap(image.scaled(imgDisNg1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
				_lastImageNg1 = image;
			}
		}
		else
		{
			if (_currentImageEnlargedDisplayIndex == 0)
			{
				_imageEnlargedDisplay->setShowImg(image);
			}
			if (isbad && _currentImageEnlargedDisplayIndex == 2)
			{
				_imageEnlargedDisplay->setShowImg(image);
				_lastImageNg1 = image;
			}
		}
		_lastImage1 = image;
	}
	else if (index == 2)
	{
		if (!_isImageEnlargedDisplay)
		{
			imgDis2->setPixmap(image.scaled(imgDis2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
			if (isbad)
			{
				imgDisNg2->setPixmap(image.scaled(imgDisNg2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
				_lastImageNg2 = image;
			}
		}
		else
		{
			if (_currentImageEnlargedDisplayIndex == 1)
			{
				_imageEnlargedDisplay->setShowImg(image);
			}
			if (isbad && _currentImageEnlargedDisplayIndex == 3)
			{
				_imageEnlargedDisplay->setShowImg(image);
				_lastImageNg2 = image;
			}
		}
		_lastImage2 = image;
	}
}

void WetPapers::lb_title_clicked()
{
	if (0 != minimizeCount)
	{
		minimizeCount--;
	}
	else if (0 >= minimizeCount)
	{
		// 最小化主窗体
		this->showMinimized();

		auto& _dlgProductSet = Modules::getInstance().uiModule._dlgProductSet;
		auto& _dlgProductScore = Modules::getInstance().uiModule._dlgProductScore;

		// 最小化所有子窗体（如果已创建且可见）
		if (_dlgProductSet && _dlgProductSet->isVisible())
			_dlgProductSet->showMinimized();
		if (_dlgProductScore && _dlgProductScore->isVisible())
			_dlgProductScore->showMinimized();
		if (_picturesViewer && _picturesViewer->isVisible())
			_picturesViewer->showMinimized();
		if (_imageEnlargedDisplay && _imageEnlargedDisplay->isVisible())
			_imageEnlargedDisplay->showMinimized();

		minimizeCount = 3; // 重置最小化计数器
	}

}

void WetPapers::pbtn_exit_clicked()
{
#ifdef NDEBUG
	auto& _dlgCloseForm = Modules::getInstance().uiModule._dlgCloseForm;
	if (_dlgCloseForm)
	{
		_dlgCloseForm->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
		_dlgCloseForm->exec();
	}
#else
	this->close();
#endif
}

void WetPapers::pbtn_set_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		if (numKeyBord.getValue() == "1234") {
			auto& _dlgProductSet = Modules::getInstance().uiModule._dlgProductSet;
			_dlgProductSet->setFixedSize(this->width(), this->height());
			_dlgProductSet->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
			_dlgProductSet->exec();
		}
		else {
			QMessageBox::warning(this, "Error", "密码错误，请重新输入");
		}
	}
}

void WetPapers::pbtn_score_clicked()
{
	auto& _dlgProductScore = Modules::getInstance().uiModule._dlgProductScore;
	_dlgProductScore->setFixedSize(this->width(), this->height());
	_dlgProductScore->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	_dlgProductScore->exec();
}

void WetPapers::rbtn_debug_checked(bool checked)
{
	auto isRuning = ui->rbtn_removeFunc->isChecked();
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& camera2 = Modules::getInstance().cameraModule.camera2;
	if (!isRuning) {
		if (checked) {
			runningState = RunningState::Debug;
			if (camera1)
			{
				camera1->setTriggerState(false);
				camera1->setFrameRate(5);
			}
			if (camera2)
			{
				camera2->setTriggerState(false);
				camera2->setFrameRate(5);
			}
			ui->rbtn_takePicture->setChecked(false);
		}
		else {
			runningState = RunningState::Stop;
		}
		ui->ckb_shibiekuang->setVisible(checked);
		ui->ckb_wenzi->setVisible(checked);
	}
	else {
		ui->rbtn_debug->setChecked(false);
	}

	emit changeToDebugMode();
}

void WetPapers::pbtn_openSaveLocation_clicked()
{
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
	QString imageSavePath = imageSaveEngine->getRootPath();

	_picturesViewer->setRootPath(imageSavePath);
	_picturesViewer->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	_picturesViewer->show();
}

void WetPapers::rbtn_takePicture_checked()
{
	if (ui->rbtn_debug->isChecked() == true)
	{
		ui->rbtn_takePicture->setChecked(false);
	}
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.wetPapersConfig;
	auto& isTakePictures = Modules::getInstance().runtimeInfoModule.isTakePictures;
	wetPapersConfig.isSaveImg = ui->rbtn_takePicture->isChecked();
	isTakePictures = ui->rbtn_takePicture->isChecked();
}

void WetPapers::rbtn_removeFunc_checked(bool checked)
{
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& camera2 = Modules::getInstance().cameraModule.camera2;
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;

	if (checked)
	{
		runningState = RunningState::OpenRemoveFunc;
		if (camera1)
		{
			camera1->setTriggerState(true);
			camera1->setFrameRate(50);
		}
		if (camera2)
		{
			camera2->setTriggerState(true);
			camera2->setFrameRate(50);
		}
		ui->rbtn_debug->setChecked(false);
		ui->ckb_shibiekuang->setVisible(false);
		ui->ckb_wenzi->setVisible(false);
	}
	else
	{
		runningState = RunningState::Stop;
	}

	emit changeToRemoveMode();
}

void WetPapers::ckb_shibiekuang_checked(bool checked)
{
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.wetPapersConfig;
	wetPapersConfig.isshibiekuang = ui->ckb_shibiekuang->isChecked();

	emit shibiekuangChanged();
}

void WetPapers::ckb_wenzi_checked(bool checked)
{
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.wetPapersConfig;
	wetPapersConfig.iswenzi = ui->ckb_wenzi->isChecked();

	emit wenziChanged();
}

void WetPapers::pbtn_resetProduct_clicked()
{
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	statisticalInfo.produceCount = 0;
	statisticalInfo.wasteCount = 0;
	statisticalInfo.productionYield = 0.0f;
	onUpdateStatisticalInfoUI();
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.wetPapersConfig;
	wetPapersConfig.totalProductionVolume = 0;
	wetPapersConfig.totalDefectiveVolume = 0;
	wetPapersConfig.productionYield = 0.0f;
}

void WetPapers::imgDis1_clicked()
{
	if (!_lastImage1.isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImage1);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 0;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void WetPapers::imgDis2_clicked()
{
	if (!_lastImage2.isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImage2);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 1;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void WetPapers::imgDisNg1_clicked()
{
	if (!_lastImageNg1.isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNg1);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 2;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void WetPapers::imgDisNg2_clicked()
{
	if (!_lastImageNg2.isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNg2);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 3;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

#ifdef BUILD_WITHOUT_HARDWARE
void WetPapers::build_DetachTestImgThread()
{
	auto& globalThread = GlobalThread::getInstance();
	globalThread.build_DetachTestImgThread();

	QObject::connect(globalThread.detachTestImgThread, &DetachTestImgThread::frameCaptured,
		globalThread.modelCamera1.get(), &ImageProcessingModuleWetPapers::onFrameCaptured);
	/*QObject::connect(globalThread.detachTestImgThread, &DetachTestImgThread::frameCaptured,
		globalThread.modelCamera2.get(), &ImageProcessingModuleWetPapers::onFrameCaptured);*/
}

void WetPapers::destroy_DetachTestImgThread()
{
	auto& globalThread = GlobalThread::getInstance();
	globalThread.destroy_DetachTestImgThread();
}

void WetPapers::start_DetachTestImgThread()
{
	auto& globalThread = GlobalThread::getInstance();
	if (globalThread.detachTestImgThread)
	{
		globalThread.detachTestImgThread->startThread();
	}
}

void WetPapers::stop_DetachTestImgThread()
{
	auto& globalThread = GlobalThread::getInstance();
	if (globalThread.detachTestImgThread)
	{
		globalThread.detachTestImgThread->stopThread();
	}
}
#endif
