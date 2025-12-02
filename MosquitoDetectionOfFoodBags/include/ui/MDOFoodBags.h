#pragma once

#include <QCheckBox>
#include <QMainWindow>
#include "PictureViewerThumbnails.h"
#include "ImageEnlargedDisplay.h"
#include"rqw_LabelClickable.h"
#include <halconcpp/HalconCpp.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MDOFoodBagsClass; };
QT_END_NAMESPACE

class MDOFoodBags : public QMainWindow
{
	Q_OBJECT
public:
	MDOFoodBags(QWidget *parent = nullptr);
	~MDOFoodBags();
#ifdef BUILD_WITHOUT_HARDWARE
public:
	QCheckBox* cBox_testPushImg{ nullptr };
public slots:
	void cBox_testPushImg_checked(bool checked);
#endif
public:
	void build_ui();
	void build_connect();
	void build_MDOFoodBagsData();
	void ini_clickableTitle();

public:
	void initializeComponents();
	void destroyComponents();
public:
	void getCameraStateAndUpdateUi();

	void getMotionStateAndUpdateUi();

	void build_ImageEnlargedDisplay();
	void destroy_ImageEnlargedDisplay();

public slots:
	void onCameraDisplay(QPixmap image, size_t index, bool isbad, bool isSmallNgImg);

	void updateCameraLabelState(int cameraIndex, bool state);

	void onUpdateStatisticalInfoUI();
private slots:
	void lb_title_clicked();
private slots:
	void pbtn_exit_clicked();
	void pbtn_set_clicked();
	void rbtn_debug_checked(bool checked);
	void pbtn_openSaveLocation_clicked();
	void rbtn_takePicture_checked();
	void rbtn_removeFunc_checked(bool checked);
	void ckb_shibiekuang_checked(bool checked);
	void ckb_wenzi_checked(bool checked);
	void pbtn_resetProduct_clicked();
	void pbtn_start_clicked();
	void pbtn_limit_clicked();
	void pbtn_baoguang_clicked();

signals:
	void shibiekuangChanged();
	
public:
	static void setModelHImage(const HalconCpp::HObject& img);
	static std::shared_ptr<const HalconCpp::HObject> getModelHImage();
	static void setModelQImage(const QImage& img);
	static std::shared_ptr<const QImage> getModelQImage();
	static void setIsModelImageLoaded(bool isLoaded);
	static bool getIsModelImageLoaded();
private:
	static std::shared_ptr<const HalconCpp::HObject> modelHImage;
	static std::shared_ptr<const QImage> modelQImage;
	static std::atomic_bool isModelImageLoaded;

private:
	bool _isImageEnlargedDisplay{ false };
	int _currentImageEnlargedDisplayIndex{ 0 };
	std::map<int, QString> _workStationTitleMap{};
	std::map<int, QPixmap> _workStationImageMap{};

	QPixmap _lastImage1{};
	std::vector<QPixmap> _lastImageNgList;  // 存储最多5张NG图片
	static constexpr size_t MAX_NG_IMAGES = 5;

	void processLastImageNg(const QPixmap& img);
	void updateNgImageDisplay(size_t index, const QPixmap& pixmap);
	void updateAllNgImageDisplays();
	rw::rqw::ClickableLabel* getNgLabelByIndex(size_t index);

	rw::rqw::ClickableLabel* imgDis1 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg1 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg2 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg3 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg4 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg5 = nullptr;
private slots:
	void imgDis1_clicked();
	void imgDisNg1_clicked();
	void imgDisNg2_clicked();
	void imgDisNg3_clicked();
	void imgDisNg4_clicked();
	void imgDisNg5_clicked();
private:
	Ui::MDOFoodBagsClass* ui;
	int minimizeCount{ 3 };
private:
	PictureViewerThumbnails* _picturesViewer = nullptr;
	ImageEnlargedDisplay* _imageEnlargedDisplay = nullptr;
	rw::rqw::ClickableLabel* clickableTitle = nullptr;

#ifdef BUILD_WITHOUT_HARDWARE
#endif
};
