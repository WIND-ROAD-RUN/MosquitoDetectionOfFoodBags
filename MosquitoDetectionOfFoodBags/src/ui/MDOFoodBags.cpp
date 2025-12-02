#include "ui_MDOFoodBags.h"

#include "MDOFoodBags.h"
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>

#include "DlgCloseForm.h"
#include "DlgProductLimit.h"
#include "DlgProductScore.h"
#include "DlgProductSet.h"
#include "Modules.hpp"
#include "NumberKeyboard.h"
#include"Utilty.hpp"

std::shared_ptr<const HalconCpp::HObject> MDOFoodBags::modelHImage{};
std::shared_ptr<const QImage> MDOFoodBags::modelQImage{};
std::atomic_bool MDOFoodBags::isModelImageLoaded{ false };

MDOFoodBags::MDOFoodBags(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MDOFoodBagsClass())
{
	ui->setupUi(this);

	initializeComponents();
}

MDOFoodBags::~MDOFoodBags()
{
	destroyComponents();
	Modules::getInstance().stop();
	Modules::getInstance().destroy();
	delete ui;
}

#ifdef BUILD_WITHOUT_HARDWARE
void MDOFoodBags::cBox_testPushImg_checked(bool checked)
{
	if (checked)
	{
		Modules::getInstance().test_module.testImgPush = true;
	}
	else
	{
		Modules::getInstance().test_module.testImgPush = false;
	}
}
#endif

void MDOFoodBags::build_ui()
{
	build_MDOFoodBagsData();

#ifdef BUILD_WITHOUT_HARDWARE
	cBox_testPushImg = new QCheckBox(this);
	cBox_testPushImg->setText("图像推送状态");
	ui->gBox_infor->layout()->addWidget(cBox_testPushImg);
	QObject::connect(cBox_testPushImg, &QCheckBox::clicked,
		this, &MDOFoodBags::cBox_testPushImg_checked);
#endif
}

void MDOFoodBags::build_connect()
{
	connect(ui->pbtn_exit, &QPushButton::clicked, this, &MDOFoodBags::pbtn_exit_clicked);
	connect(ui->pbtn_set, &QPushButton::clicked, this, &MDOFoodBags::pbtn_set_clicked);
	connect(ui->pbtn_start, &QPushButton::clicked, this, &MDOFoodBags::pbtn_start_clicked);
	connect(ui->rbtn_debug, &QRadioButton::toggled, this, &MDOFoodBags::rbtn_debug_checked);
	connect(ui->rbtn_takePicture, &QRadioButton::toggled, this, &MDOFoodBags::rbtn_takePicture_checked);
	connect(ui->rbtn_removeFunc, &QRadioButton::toggled, this, &MDOFoodBags::rbtn_removeFunc_checked);
	connect(ui->ckb_shibiekuang, &QCheckBox::toggled, this, &MDOFoodBags::ckb_shibiekuang_checked);
	connect(ui->ckb_wenzi, &QCheckBox::toggled, this, &MDOFoodBags::ckb_wenzi_checked);
	connect(ui->pbtn_openSaveLocation, &QPushButton::clicked, this, &MDOFoodBags::pbtn_openSaveLocation_clicked);
	connect(ui->pbtn_limit, &QPushButton::clicked, this, &MDOFoodBags::pbtn_limit_clicked);
	connect(ui->pbtn_baoguang, &QPushButton::clicked, this, &MDOFoodBags::pbtn_baoguang_clicked);


	// 连接显示标题
	QObject::connect(clickableTitle, &rw::rqw::ClickableLabel::clicked,
		this, &MDOFoodBags::lb_title_clicked);
	connect(ui->pbtn_resetProduct, &QPushButton::clicked, this, &MDOFoodBags::pbtn_resetProduct_clicked);
}

void MDOFoodBags::build_MDOFoodBagsData()
{
	auto& mainWindowConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;
	mainWindowConfig.isDebug = false;
	mainWindowConfig.isDefect = true;		// 默认开启剔废
	rbtn_removeFunc_checked(true);
	mainWindowConfig.isshibiekuang = true;
	mainWindowConfig.iswenzi = true;

	/*ui->label_produceTotalValue->setText(QString::number(wetPapersConfig.totalProductionVolume));
	ui->label_wasteProductsValue->setText(QString::number(wetPapersConfig.totalDefectiveVolume));
	ui->label_productionYieldValue->setText(QString::number(wetPapersConfig.productionYield));*/
	ui->rbtn_takePicture->setChecked(mainWindowConfig.isSaveImg);
	ui->rbtn_removeFunc->setChecked(mainWindowConfig.isDefect);
	ui->ckb_shibiekuang->setChecked(mainWindowConfig.isshibiekuang);
	ui->ckb_wenzi->setChecked(mainWindowConfig.iswenzi);
	ui->pbtn_baoguang->setText(QString::number(mainWindowConfig.baoguang));

	// release版本
#ifdef NDEBUG
	wetPapersConfig.isSaveImg = true;
	ui->rbtn_takePicture->setChecked(true);
	ui->rbtn_debug->setVisible(false);
#endif

	// 不启用限位窗体
	ui->pbtn_limit->setVisible(false);

	Modules::getInstance().runtimeInfoModule.isTakePictures = mainWindowConfig.isSaveImg;

	// 初始化图像查看器
	_picturesViewer = new PictureViewerThumbnails(this);

	ini_clickableTitle();
}

void MDOFoodBags::ini_clickableTitle()
{
	// 初始化标题label
	clickableTitle = new rw::rqw::ClickableLabel(this);
	auto layoutTitle = ui->groupBox_head->layout();
	layoutTitle->replaceWidget(ui->label_title, clickableTitle);
	delete ui->label_title;
	clickableTitle->setText("食品袋蚊虫检测");
	clickableTitle->setStyleSheet("QLabel {font-size: 30px;font-weight: bold;color: rgb(255, 255, 255);padding: 5px 5px;border-bottom: 2px solid #cccccc;}");
}

void MDOFoodBags::initializeComponents()
{
	build_ui();

	build_ImageEnlargedDisplay();

	getMotionStateAndUpdateUi();

	getCameraStateAndUpdateUi();

	build_connect();

	pbtn_start_clicked();

#ifdef BUILD_WITHOUT_HARDWARE
#endif
}

void MDOFoodBags::destroyComponents()
{
#ifdef BUILD_WITHOUT_HARDWARE
#endif

	destroy_ImageEnlargedDisplay();
}

void MDOFoodBags::getCameraStateAndUpdateUi()
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

void MDOFoodBags::getMotionStateAndUpdateUi()
{
	auto& motionControllerModule = Modules::getInstance().motionControllerModule;
	auto& isBuildZmotion = motionControllerModule.isConnectMotion;
	updateCameraLabelState(0, isBuildZmotion);
}

void MDOFoodBags::build_ImageEnlargedDisplay()
{
	_lastImageNgList.resize(MAX_NG_IMAGES);

	imgDis1 = new rw::rqw::ClickableLabel(this);
	imgDis1->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg1 = new rw::rqw::ClickableLabel(this);
	imgDisNg1->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg2 = new rw::rqw::ClickableLabel(this);
	imgDisNg2->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg3 = new rw::rqw::ClickableLabel(this);
	imgDisNg3->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg4 = new rw::rqw::ClickableLabel(this);
	imgDisNg4->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	imgDisNg5 = new rw::rqw::ClickableLabel(this);
	imgDisNg5->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgDisplay_1, imgDis1);

	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgNgDisplay_1, imgDisNg1);
	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgNgDisplay_2, imgDisNg2);
	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgNgDisplay_3, imgDisNg3);
	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgNgDisplay_4, imgDisNg4);
	ui->gBoix_ImageDisplay->layout()->replaceWidget(ui->label_imgNgDisplay_5, imgDisNg5);

	delete ui->label_imgDisplay_1;
	delete ui->label_imgNgDisplay_1;
	delete ui->label_imgNgDisplay_2;
	delete ui->label_imgNgDisplay_3;
	delete ui->label_imgNgDisplay_4;
	delete ui->label_imgNgDisplay_5;

	QObject::connect(imgDis1, &rw::rqw::ClickableLabel::clicked
		, this, &MDOFoodBags::imgDis1_clicked);
	QObject::connect(imgDisNg1, &rw::rqw::ClickableLabel::clicked
		, this, &MDOFoodBags::imgDisNg1_clicked);
	QObject::connect(imgDisNg2, &rw::rqw::ClickableLabel::clicked
		, this, &MDOFoodBags::imgDisNg2_clicked);
	QObject::connect(imgDisNg3, &rw::rqw::ClickableLabel::clicked
		, this, &MDOFoodBags::imgDisNg3_clicked);
	QObject::connect(imgDisNg4, &rw::rqw::ClickableLabel::clicked
		, this, &MDOFoodBags::imgDisNg4_clicked);
	QObject::connect(imgDisNg5, &rw::rqw::ClickableLabel::clicked
		, this, &MDOFoodBags::imgDisNg5_clicked);

	_workStationTitleMap = {
		{0,"一号工位"},
		{1,"NG一号图片"},
		{2,"NG二号图片"},
		{3,"NG三号图片"},
		{4,"NG四号图片"},
		{5,"NG五号图片"}
	};

	for (int i = 0; i < 6; ++i) {
		_workStationImageMap[i] = QPixmap();
	}

	_imageEnlargedDisplay = new ImageEnlargedDisplay(this);
	_imageEnlargedDisplay->setMonitorValue(&_isImageEnlargedDisplay);
	_imageEnlargedDisplay->setMonitorDisImgIndex(&_currentImageEnlargedDisplayIndex);
	_imageEnlargedDisplay->initWorkStationTitleMap(_workStationTitleMap);
	_imageEnlargedDisplay->initWorkStationImageMap(_workStationImageMap);
	_imageEnlargedDisplay->setNum(6);
	_imageEnlargedDisplay->show();
	_imageEnlargedDisplay->close();
}

void MDOFoodBags::destroy_ImageEnlargedDisplay()
{
	if (_imageEnlargedDisplay)
	{
		_imageEnlargedDisplay->close();
		delete _imageEnlargedDisplay;
		_imageEnlargedDisplay = nullptr;
	}
}

void MDOFoodBags::updateCameraLabelState(int cameraIndex, bool state)
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
	default:
		break;
	}
}

void MDOFoodBags::onUpdateStatisticalInfoUI()
{
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	/*ui->label_produceTotalValue->setText(QString::number(statisticalInfo.produceCount.load()));
	ui->label_wasteProductsValue->setText(QString::number(statisticalInfo.wasteCount.load()));
	ui->label_productionYieldValue->setText(QString::number(statisticalInfo.productionYield.load(), 'f', 2) + "%");*/
}

void MDOFoodBags::onCameraDisplay(QPixmap image, size_t index, bool isbad, bool isSmallNgImg)
{
	if (index == 1)
	{
		if (!isSmallNgImg)
		{
			_workStationImageMap[0] = image;
			_imageEnlargedDisplay->initWorkStationImageMap(_workStationImageMap);

			if (!_isImageEnlargedDisplay)
			{
				imgDis1->setPixmap(image.scaled(imgDis1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
			}
			else
			{
				if (_currentImageEnlargedDisplayIndex == 0)
				{
					_imageEnlargedDisplay->setShowImg(image);
				}
			}
			_lastImage1 = image;
		}
		
		if (isbad)
		{
			processLastImageNg(image);
		}
	}
}

void MDOFoodBags::lb_title_clicked()
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

void MDOFoodBags::pbtn_exit_clicked()
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

void MDOFoodBags::pbtn_set_clicked()
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

void MDOFoodBags::rbtn_debug_checked(bool checked)
{
	auto isRuning = ui->rbtn_removeFunc->isChecked();
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	if (!isRuning) {
		if (checked) {
			runningState = RunningState::Debug;
			if (camera1)
			{
				camera1->setFrameTriggered(false);
				camera1->setLineTriggered(false);
				camera1->setLineHeight(500);
			}
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
}

void MDOFoodBags::pbtn_openSaveLocation_clicked()
{
	auto& imageSaveEngine = Modules::getInstance().imgSaveModule.imageSaveEngine;
	QString imageSavePath = imageSaveEngine->getRootPath();

	_picturesViewer->setRootPath(imageSavePath);
	_picturesViewer->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	_picturesViewer->show();
}

void MDOFoodBags::rbtn_takePicture_checked()
{
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;
	auto& isTakePictures = Modules::getInstance().runtimeInfoModule.isTakePictures;
	wetPapersConfig.isSaveImg = ui->rbtn_takePicture->isChecked();
	isTakePictures = ui->rbtn_takePicture->isChecked();
}

void MDOFoodBags::rbtn_removeFunc_checked(bool checked)
{
	auto& camera1 = Modules::getInstance().cameraModule.camera1;
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;

	if (checked)
	{
		runningState = RunningState::OpenRemoveFunc;
		if (camera1)
		{
			camera1->setFrameTriggered(false);
			camera1->setLineTriggered(true);
			auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
			double xiangsudangliang = setConfig.xiangSuDangLiang;
			double xiangjichufachangdu = setConfig.xiangjichufachangdu;
			double hanggao = xiangjichufachangdu / xiangsudangliang;

			camera1->setLineHeight((int)hanggao);
		}
		ui->rbtn_debug->setChecked(false);
		ui->ckb_shibiekuang->setVisible(false);
		ui->ckb_wenzi->setVisible(false);
	}
	else
	{
		runningState = RunningState::Stop;
	}
}

void MDOFoodBags::ckb_shibiekuang_checked(bool checked)
{
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;
	wetPapersConfig.isshibiekuang = ui->ckb_shibiekuang->isChecked();

	emit shibiekuangChanged();
}

void MDOFoodBags::ckb_wenzi_checked(bool checked)
{
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;
	wetPapersConfig.iswenzi = ui->ckb_wenzi->isChecked();
}

void MDOFoodBags::pbtn_resetProduct_clicked()
{
	auto& statisticalInfo = Modules::getInstance().runtimeInfoModule.statisticalInfo;
	statisticalInfo.produceCount = 0;
	statisticalInfo.wasteCount = 0;
	statisticalInfo.productionYield = 0.0f;
	onUpdateStatisticalInfoUI();
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;
	wetPapersConfig.totalProductionVolume = 0;
	wetPapersConfig.totalDefectiveVolume = 0;
	wetPapersConfig.productionYield = 0.0f;
}

void MDOFoodBags::pbtn_start_clicked()
{
	setIsModelImageLoaded(false);

	auto& camera = Modules::getInstance().cameraModule.camera1;
	if (camera)
	{
		camera->softwareTrigger();
	}
}

void MDOFoodBags::pbtn_limit_clicked()
{
	auto& _dlgProductLimit = Modules::getInstance().uiModule._dlgProductLimit;
	_dlgProductLimit->setFixedSize(this->width(), this->height());
	_dlgProductLimit->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	_dlgProductLimit->exec();
}

void MDOFoodBags::pbtn_baoguang_clicked()
{
	NumberKeyboard numKeyBord;
	numKeyBord.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	auto isAccept = numKeyBord.exec();
	if (isAccept == QDialog::Accepted)
	{
		auto value = numKeyBord.getValue();
		if (value.toDouble() < 0)
		{
			QMessageBox::warning(this, "提示", "请输入大于0的数值");
			return;
		}
		auto& mainWindowConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;
		ui->pbtn_baoguang->setText(value);
		mainWindowConfig.baoguang = value.toDouble();
		auto& camera1 = Modules::getInstance().cameraModule.camera1;
		if (camera1)
		{
			camera1->setExposureTime(static_cast<size_t>(value.toDouble()));
		}
	}
}

void MDOFoodBags::setModelHImage(const HalconCpp::HObject& img)
{
	auto sp = std::make_shared<HalconCpp::HObject>(img);
	std::shared_ptr<const HalconCpp::HObject> csp = sp;

	std::atomic_store_explicit(&modelHImage, csp, std::memory_order_release);
	isModelImageLoaded.store(img.IsInitialized(), std::memory_order_relaxed);
}

std::shared_ptr<const HalconCpp::HObject> MDOFoodBags::getModelHImage()
{
	return std::atomic_load_explicit(&modelHImage, std::memory_order_acquire);
}

void MDOFoodBags::setModelQImage(const QImage& img)
{
	auto sp = std::make_shared<QImage>(img);
	std::shared_ptr<const QImage> csp = sp;
	std::atomic_store_explicit(&modelQImage, csp, std::memory_order_release);
}

std::shared_ptr<const QImage> MDOFoodBags::getModelQImage()
{
	return std::atomic_load_explicit(&modelQImage, std::memory_order_acquire);
}

void MDOFoodBags::setIsModelImageLoaded(bool isLoaded)
{
	isModelImageLoaded.store(isLoaded, std::memory_order_relaxed);
}

bool MDOFoodBags::getIsModelImageLoaded()
{
	return isModelImageLoaded.load(std::memory_order_relaxed);
}

void MDOFoodBags::processLastImageNg(const QPixmap& img)
{
	if (img.isNull()) {
		return;
	}

	// 查找第一个空位置（isNull的位置）
	for (size_t i = 0; i < _lastImageNgList.size(); ++i) {
		if (_lastImageNgList[i].isNull()) {
			_lastImageNgList[i] = img;
			_workStationImageMap[i + 1] = img;
			_imageEnlargedDisplay->initWorkStationImageMap(_workStationImageMap);
			updateNgImageDisplay(i, img);
			return;
		}
	}

	// 如果没有空位置且未满，则添加新元素
	if (_lastImageNgList.size() < MAX_NG_IMAGES) {
		_lastImageNgList.push_back(img);
		size_t newIndex = _lastImageNgList.size() - 1;
		_workStationImageMap[newIndex + 1] = img;
		_imageEnlargedDisplay->initWorkStationImageMap(_workStationImageMap);
		updateNgImageDisplay(_lastImageNgList.size() - 1, img);
	}
	else {
		// 如果已满，移除最早的图片，添加新图片（队列行为）
		_lastImageNgList.erase(_lastImageNgList.begin());
		_lastImageNgList.push_back(img);
		for (size_t i = 0; i < _lastImageNgList.size(); ++i) {
			_workStationImageMap[i + 1] = _lastImageNgList[i];
		}
		_imageEnlargedDisplay->initWorkStationImageMap(_workStationImageMap);
		updateAllNgImageDisplays();
	}
}

void MDOFoodBags::updateNgImageDisplay(size_t index, const QPixmap& pixmap)
{
	if (!_isImageEnlargedDisplay) {
		rw::rqw::ClickableLabel* label = getNgLabelByIndex(index);
		if (label) {
			label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	}
	else if (static_cast<size_t>(_currentImageEnlargedDisplayIndex) == index + 1) {
		_imageEnlargedDisplay->setShowImg(pixmap);
	}
}

void MDOFoodBags::updateAllNgImageDisplays()
{
	for (size_t i = 0; i < _lastImageNgList.size(); ++i) {
		updateNgImageDisplay(i, _lastImageNgList[i]);
	}
}

rw::rqw::ClickableLabel* MDOFoodBags::getNgLabelByIndex(size_t index)
{
	switch (index) {
	case 0: return imgDisNg1;
	case 1: return imgDisNg2;
	case 2: return imgDisNg3;
	case 3: return imgDisNg4;
	case 4: return imgDisNg5;
	default: return nullptr;
	}
}

void MDOFoodBags::imgDis1_clicked()
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

void MDOFoodBags::imgDisNg1_clicked()
{
	if (!_lastImageNgList.empty() && !_lastImageNgList[0].isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNgList[0]);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 1;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void MDOFoodBags::imgDisNg2_clicked()
{
	if (!_lastImageNgList.empty() && !_lastImageNgList[1].isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNgList[1]);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 2;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void MDOFoodBags::imgDisNg3_clicked()
{
	if (!_lastImageNgList.empty() && !_lastImageNgList[2].isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNgList[2]);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 3;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void MDOFoodBags::imgDisNg4_clicked()
{
	if (!_lastImageNgList.empty() && !_lastImageNgList[3].isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNgList[3]);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 4;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}

void MDOFoodBags::imgDisNg5_clicked()
{
	if (!_lastImageNgList.empty() && !_lastImageNgList[4].isNull())
	{
		_imageEnlargedDisplay->setShowImg(_lastImageNgList[4]);
	}
	else
	{
		_imageEnlargedDisplay->clearImgDis();
	}
	_currentImageEnlargedDisplayIndex = 5;
	_imageEnlargedDisplay->setGboxTitle(_workStationTitleMap[_currentImageEnlargedDisplayIndex]);
	_imageEnlargedDisplay->show();
}
