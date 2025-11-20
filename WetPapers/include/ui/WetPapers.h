#pragma once

#include <QMainWindow>
#include "PictureViewerThumbnails.h"
#include "ImageEnlargedDisplay.h"
#include"rqw_LabelClickable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PunchPressClass; };
QT_END_NAMESPACE

class WetPapers : public QMainWindow
{
	Q_OBJECT
public:
	WetPapers(QWidget *parent = nullptr);
	~WetPapers();
public:
	void build_ui();
	void build_connect();
	void build_WetPapersData();
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
	void onCameraNGDisplay(QPixmap image, size_t index, bool isbad);

	void updateCameraLabelState(int cameraIndex, bool state);

	void onUpdateStatisticalInfoUI();
private slots:
	void lb_title_clicked();
private slots:
	void pbtn_exit_clicked();
	void pbtn_set_clicked();
	void pbtn_score_clicked();
	void rbtn_debug_checked(bool checked);
	void pbtn_openSaveLocation_clicked();
	void rbtn_takePicture_checked();
	void rbtn_removeFunc_checked(bool checked);
	void ckb_shibiekuang_checked(bool checked);
	void ckb_wenzi_checked(bool checked);
	void pbtn_resetProduct_clicked();

signals:
	void shibiekuangChanged();
	void wenziChanged();
	void changeToDebugMode();
	void changeToRemoveMode();

private:
	bool _isImageEnlargedDisplay{ false };
	int _currentImageEnlargedDisplayIndex{ 0 };
	std::map<int, QString> _workStationTitleMap{};

	QPixmap _lastImage1{};
	QPixmap _lastImage2{};
	QPixmap _lastImageNg1{};
	QPixmap _lastImageNg2{};

	rw::rqw::ClickableLabel* imgDis1 = nullptr;
	rw::rqw::ClickableLabel* imgDis2 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg1 = nullptr;
	rw::rqw::ClickableLabel* imgDisNg2 = nullptr;
private slots:
	void imgDis1_clicked();
	void imgDis2_clicked();
	void imgDisNg1_clicked();
	void imgDisNg2_clicked();
private:
	Ui::PunchPressClass* ui;
	int minimizeCount{ 3 };
private:
	PictureViewerThumbnails* _picturesViewer = nullptr;
	ImageEnlargedDisplay* _imageEnlargedDisplay = nullptr;
	rw::rqw::ClickableLabel* clickableTitle = nullptr;

#ifdef BUILD_WITHOUT_HARDWARE
	void build_DetachTestImgThread();
	void destroy_DetachTestImgThread();
	void start_DetachTestImgThread();
	void stop_DetachTestImgThread();
#endif
};
