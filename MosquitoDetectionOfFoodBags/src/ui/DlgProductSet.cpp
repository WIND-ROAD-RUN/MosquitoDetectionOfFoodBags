#include "DlgProductSet.h"
#include "ui_DlgProductSet.h"

#include <QMessageBox>
#include <QTabWidget>
#include <QtConcurrent/qtconcurrentrun.h>

#include "Modules.hpp"
#include "NumberKeyboard.h"

DlgProductSet::DlgProductSet(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::DlgProductSetClass())
{
	ui->setupUi(this);

	build_ui();

	build_connect();
}

DlgProductSet::~DlgProductSet()
{
	delete ui;
}

void DlgProductSet::build_ui()
{
	read_config();

	// 查看是否有相同的输入输出IO
	auto indicesDO = DOFindAllDuplicateIndices();
	setDOErrorInfo(indicesDO);
	auto indicesDI = DIFindAllDuplicateIndices();
	setDIErrorInfo(indicesDI);
}

void DlgProductSet::read_config()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	// 剔废持续时间
	ui->pbtn_tifeichixushijian->setText(QString::number(setConfig.tifeichixushijian));
	// 剔废时间
	ui->pbtn_tifeijuli->setText(QString::number(setConfig.tifeijuli));

	// 一工位
	ui->pbtn_shangxiasuojin->setText(QString::number(setConfig.shangxiasuojin));
	ui->pbtn_zuoxianwei->setText(QString::number(setConfig.zuoXianWei));
	ui->pbtn_youxianwei->setText(QString::number(setConfig.youXianWei));
	ui->pbtn_zengyi->setText(QString::number(setConfig.zengyi));
	ui->pbtn_xiangsudangliang->setText(QString::number(setConfig.xiangSuDangLiang));
	ui->pbtn_houfenpin->setText(QString::number(setConfig.houFenPin));
	ui->pbtn_chengfaqi->setText(QString::number(setConfig.chengFaQi));

	// 调试模式默认为关闭
	setConfig.debugMode = false;
	ui->cbox_debugMode->setChecked(setConfig.debugMode);

	// 基本功能
	ui->cbox_yundongkongzhiqichonglian->setChecked(setConfig.yundongkongzhiqichonglian);

	// 基本参数
	ui->btn_xiangjichufachangdu->setText(QString::number(setConfig.xiangjichufachangdu));
	ui->btn_meizhuanmaichongshu->setText(QString::number(setConfig.meizhuanmaichongshu));
	ui->btn_shedingzhouchang->setText(QString::number(setConfig.shedingzhouchang));
	ui->btn_tifeijishu->setText(QString::number(setConfig.tifeijishu));
	ui->btn_liangpinjishu->setText(QString::number(setConfig.liangpinjishu));
	ui->btn_baojingchixushijian->setText(QString::number(setConfig.baojingchixushijian));

	// 设置IO
	ui->btn_setDIjiaodaitiewan->setText(QString::number(setConfig.jiaodaitiewanIN));

	ui->btn_setDOtifeixinhao->setText(QString::number(setConfig.tifeixinhaoOUT));
	ui->btn_setDObaojing->setText(QString::number(setConfig.baojingOUT));
	ui->btn_setDOlvdeng->setText(QString::number(setConfig.lvdengOUT));
	ui->btn_setDOhongdeng->setText(QString::number(setConfig.hongdengOUT));

	// 默认显示第一个
	ui->tabWidget->setCurrentIndex(0);

	// 参数设置
	ui->btn_wenchongzuidahuiduchazhi->setText(QString::number(setConfig.wenchongzuidahuiduchazhi));
	ui->btn_wenchongzuixiaohuiduchazhi->setText(QString::number(setConfig.wenchongzuixiaohuiduchazhi));
	ui->btn_wenchongzuidamianji->setText(QString::number(setConfig.wenchongzuidamianji));
	ui->btn_wenchongzuixiaomianji->setText(QString::number(setConfig.wenchongzuixiaomianji));
	ui->btn_maofazuidahuiduchazhi->setText(QString::number(setConfig.maofazuidahuiduchazhi));
	ui->btn_maofazuixiaohuiduchazhi->setText(QString::number(setConfig.maofazuixiaohuiduchazhi));
	ui->btn_maofazuidamianji->setText(QString::number(setConfig.maofazuidamianji));
	ui->btn_maofazuixiaomianji->setText(QString::number(setConfig.maofazuixiaomianji));

	updateControlLines();

	initDOCheckItems();
	initDICheckItems();
}

void DlgProductSet::build_connect()
{
	QObject::connect(ui->pbtn_tifeichixushijian, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_tifeichixushijian_clicked);
	QObject::connect(ui->pbtn_tifeijuli, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_tifeijuli_clicked);
	QObject::connect(ui->pbtn_shangxiasuojin, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_shangxiasuojin_clicked);
	QObject::connect(ui->pbtn_zuoxianwei, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_zuoxianwei_clicked);
	QObject::connect(ui->pbtn_youxianwei, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_youxianwei_clicked);
	QObject::connect(ui->pbtn_zengyi, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_zengyi_clicked);
	QObject::connect(ui->pbtn_xiangsudangliang, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_xiangsudangliang_clicked);
	QObject::connect(ui->pbtn_houfenpin, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_houfenpin_clicked);
	QObject::connect(ui->pbtn_chengfaqi, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_chengfaqi_clicked);

	QObject::connect(ui->cbox_debugMode, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_debugMode_checked);
	QObject::connect(ui->pbtn_close, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_close_clicked);
	QObject::connect(ui->cbox_yundongkongzhiqichonglian, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_yundongkongzhiqichonglian_checked);

	// 基本参数
	QObject::connect(ui->btn_xiangjichufachangdu, &QPushButton::clicked,
		this, &DlgProductSet::btn_xiangjichufachangdu_clicked);
	QObject::connect(ui->btn_meizhuanmaichongshu, &QPushButton::clicked,
		this, &DlgProductSet::btn_meizhuanmaichongshu_clicked);
	QObject::connect(ui->btn_shedingzhouchang, &QPushButton::clicked,
		this, &DlgProductSet::btn_shedingzhouchang_clicked);
	QObject::connect(ui->btn_tifeijishu, &QPushButton::clicked,
		this, &DlgProductSet::btn_tifeijishu_clicked);
	QObject::connect(ui->btn_liangpinjishu, &QPushButton::clicked,
		this, &DlgProductSet::btn_liangpinjishu_clicked);
	QObject::connect(ui->btn_baojingchixushijian, &QPushButton::clicked,
		this, &DlgProductSet::btn_baojingchixushijian_clicked);

	// 监控IO
	QObject::connect(ui->tabWidget, &QTabWidget::currentChanged,
		this, &DlgProductSet::tabWidget_indexChanged);
	QObject::connect(ui->cbox_DOtifeixinhao, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_DOtifeixinhao_checked);
	QObject::connect(ui->cbox_DObaojing, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_DObaojing_checked);
	QObject::connect(ui->cbox_DOlvdeng, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_DOlvdeng_checked);
	QObject::connect(ui->cbox_DOhongdeng, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_DOhongdeng_checked);

	// 设置IO
	QObject::connect(ui->btn_setDIjiaodaitiewan, &QPushButton::clicked,
		this, &DlgProductSet::btn_setDIjiaodaitiewan_clicked);
	QObject::connect(ui->btn_setDOtifeixinhao, &QPushButton::clicked,
		this, &DlgProductSet::btn_setDOtifeixinhao_clicked);
	QObject::connect(ui->btn_setDObaojing, &QPushButton::clicked,
		this, &DlgProductSet::btn_setDObaojing_clicked);
	QObject::connect(ui->btn_setDOlvdeng, &QPushButton::clicked,
		this, &DlgProductSet::btn_setDOlvdeng_clicked);
	QObject::connect(ui->btn_setDOhongdeng, &QPushButton::clicked,
		this, &DlgProductSet::btn_setDOhongdeng_clicked);

	// 参数设置
	QObject::connect(ui->btn_wenchongzuidahuiduchazhi, &QPushButton::clicked,
		this, &DlgProductSet::btn_wenchongzuidahuiduchazhi_clicked);
	QObject::connect(ui->btn_wenchongzuixiaohuiduchazhi, &QPushButton::clicked,
		this, &DlgProductSet::btn_wenchongzuixiaohuiduchazhi_clicked);
	QObject::connect(ui->btn_wenchongzuidamianji, &QPushButton::clicked,
		this, &DlgProductSet::btn_wenchongzuidamianji_clicked);
	QObject::connect(ui->btn_wenchongzuixiaomianji, &QPushButton::clicked,
		this, &DlgProductSet::btn_wenchongzuixiaomianji_clicked);
	QObject::connect(ui->btn_maofazuidahuiduchazhi, &QPushButton::clicked,
		this, &DlgProductSet::btn_maofazuidahuiduchazhi_clicked);
	QObject::connect(ui->btn_maofazuixiaohuiduchazhi, &QPushButton::clicked,
		this, &DlgProductSet::btn_maofazuixiaohuiduchazhi_clicked);
	QObject::connect(ui->btn_maofazuidamianji, &QPushButton::clicked,
		this, &DlgProductSet::btn_maofazuidamianji_clicked);
	QObject::connect(ui->btn_maofazuixiaomianji, &QPushButton::clicked,
		this, &DlgProductSet::btn_maofazuixiaomianji_clicked);
}

void DlgProductSet::setDIErrorInfo(const std::vector<std::vector<int>>& index)
{
	ui->lb_DIjiaodaitiewan->clear();

	for (const auto& classic : index)
	{
		for (const auto& item : classic)
		{
			setDIErrorInfo(item);
		}
	}
}

void DlgProductSet::setDIErrorInfo(int index)
{
	QString text = "重复数值";
	switch (index)
	{
	case 0:
		ui->lb_DIjiaodaitiewan->setText(text);
		break;
	default:
		break;
	}
}

void DlgProductSet::updateControlLines()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	ControlLines::tifeixinhaoOut = setConfig.tifeixinhaoOUT;
	ControlLines::baojingOut = setConfig.baojingOUT;
	ControlLines::lvdengOut = setConfig.lvdengOUT;
	ControlLines::hongdengOut = setConfig.hongdengOUT;
}

void DlgProductSet::initDOCheckItems()
{
	DOCheckItems.clear();
	DOCheckItems = {
		{ &ControlLines::tifeixinhaoOut, ui->cbox_DOtifeixinhao },
		{ &ControlLines::baojingOut, ui->cbox_DObaojing },
		{&ControlLines::lvdengOut,ui->cbox_DOlvdeng},
		{&ControlLines::hongdengOut,ui->cbox_DOhongdeng}
	};
}

void DlgProductSet::initDICheckItems()
{
	DICheckItems.clear();
	DICheckItems = {
		{&ControlLines::jiaodaitiewanIn, ui->cbox_DIjiaodaitiewan}
	};
}

std::vector<std::vector<int>> DlgProductSet::DOFindAllDuplicateIndices()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	std::vector<int> values = {
		setConfig.tifeixinhaoOUT,
		setConfig.baojingOUT,
		setConfig.lvdengOUT,
		setConfig.hongdengOUT
	};

	std::unordered_map<int, std::vector<int>> valueToIndices;
	for (size_t i = 0; i < values.size(); ++i) {
		valueToIndices[values[i]].push_back(static_cast<int>(i));
	}

	std::vector<std::vector<int>> result;
	std::set<int> used; // 防止重复分组
	for (const auto& pair : valueToIndices) {
		if (pair.second.size() > 1) {
			// 只收集未被收录过的index组
			bool alreadyUsed = false;
			for (int idx : pair.second) {
				if (used.count(idx)) {
					alreadyUsed = true;
					break;
				}
			}
			if (!alreadyUsed) {
				result.push_back(pair.second);
				used.insert(pair.second.begin(), pair.second.end());
			}
		}
	}
	return result;
}

void DlgProductSet::setDOErrorInfo(const std::vector<std::vector<int>>& index)
{
	ui->lb_DOtifeixinhao->clear();
	ui->lb_DObaojing->clear();
	ui->lb_DOlvdeng->clear();
	ui->lb_DOhongdeng->clear();


	for (const auto& classic : index)
	{
		for (const auto& item : classic)
		{
			setDOErrorInfo(item);
		}
	}
}

void DlgProductSet::setDOErrorInfo(int index)
{
	QString text = "重复数值";
	switch (index)
	{
	case 0:
		ui->lb_DOtifeixinhao->setText(text);
		break;
	case 1:
		ui->lb_DObaojing->setText(text);
		break;
	case 2:
		ui->lb_DOlvdeng->setText(text);
		break;
	case 3:
		ui->lb_DOhongdeng->setText(text);
		break;
	default:
		break;
	}
}

std::vector<std::vector<int>> DlgProductSet::DIFindAllDuplicateIndices()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	std::vector<int> values = {
		setConfig.jiaodaitiewanIN
	};

	std::unordered_map<int, std::vector<int>> valueToIndices;
	for (size_t i = 0; i < values.size(); ++i) {
		valueToIndices[values[i]].push_back(static_cast<int>(i));
	}

	std::vector<std::vector<int>> result;
	std::set<int> used; // 防止重复分组
	for (const auto& pair : valueToIndices) {
		if (pair.second.size() > 1) {
			// 只收集未被收录过的index组
			bool alreadyUsed = false;
			for (int idx : pair.second) {
				if (used.count(idx)) {
					alreadyUsed = true;
					break;
				}
			}
			if (!alreadyUsed) {
				result.push_back(pair.second);
				used.insert(pair.second.begin(), pair.second.end());
			}
		}
	}
	return result;
}

void DlgProductSet::pbtn_close_clicked()
{
	auto& _isUpdateMonitorInfo = Modules::getInstance().motionControllerModule._isUpdateMonitorInfo;
	auto& monitorZMotionMonitorThread = Modules::getInstance().motionControllerModule.monitorMotionIoStateThread;
	// 关闭监控IO线程
	_isUpdateMonitorInfo = false;
	monitorZMotionMonitorThread->setRunning(false);
	ui->cbox_debugMode->setChecked(false);
	cbox_debugMode_checked(false);

	this->close();
}

void DlgProductSet::pbtn_tifeichixushijian_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_tifeichixushijian->setText(value);
		setConfig.tifeichixushijian = value.toDouble();
	}
}

void DlgProductSet::pbtn_tifeijuli_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_tifeijuli->setText(value);
		setConfig.tifeijuli = value.toDouble();
		emit tifeijuliChanged();
	}
}

void DlgProductSet::pbtn_shangxiasuojin_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_shangxiasuojin->setText(value);
		setConfig.shangxiasuojin = value.toDouble();
	}
}

void DlgProductSet::pbtn_zuoxianwei_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_zuoxianwei->setText(value);
		setConfig.zuoXianWei = value.toDouble();
	}
}

void DlgProductSet::pbtn_youxianwei_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_youxianwei->setText(value);
		setConfig.youXianWei = value.toDouble();
	}
}

void DlgProductSet::pbtn_zengyi_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		auto& camera1 = Modules::getInstance().cameraModule.camera1;
		ui->pbtn_zengyi->setText(value);
		setConfig.zengyi = value.toDouble();
		if (camera1)
		{
			camera1->setGain(static_cast<size_t>(value.toDouble()));
		}
	}
}

void DlgProductSet::pbtn_xiangsudangliang_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->pbtn_xiangsudangliang->setText(value);
		setConfig.xiangSuDangLiang = value.toDouble();
		emit pixToWorldChanged();
	}
}

void DlgProductSet::pbtn_houfenpin_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		auto& camera1 = Modules::getInstance().cameraModule.camera1;
		ui->pbtn_houfenpin->setText(value);
		setConfig.houFenPin = value.toDouble();
		if (camera1)
		{
			camera1->setPostDivider(static_cast<size_t>(value.toDouble()));
		}
	}
}

void DlgProductSet::pbtn_chengfaqi_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		auto& camera1 = Modules::getInstance().cameraModule.camera1;
		ui->pbtn_chengfaqi->setText(value);
		setConfig.chengFaQi = value.toDouble();
		if (camera1)
		{
			camera1->setMultiplier(static_cast<size_t>(value.toDouble()));
		}
	}
}

void DlgProductSet::cbox_debugMode_checked(bool ischecked)
{
	auto& monitorZMotionMonitorThread = Modules::getInstance().motionControllerModule.monitorMotionIoStateThread;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto& isDebug = setConfig.debugMode;
	isDebug = ischecked;
	isDebugIO = ischecked;
	if (isDebugIO)
	{
		ui->cbox_DOtifeixinhao->setChecked(false);
		ui->cbox_DObaojing->setChecked(false);
		ui->cbox_DOlvdeng->setChecked(false);
		ui->cbox_DOhongdeng->setChecked(false);

		ui->cbox_DOtifeixinhao->setEnabled(true);
		ui->cbox_DObaojing->setEnabled(true);
		ui->cbox_DOlvdeng->setEnabled(true);
		ui->cbox_DOhongdeng->setEnabled(true);

		monitorZMotionMonitorThread->setRunning(false);
	}
	else
	{
		ui->cbox_DOtifeixinhao->setChecked(false);
		ui->cbox_DObaojing->setChecked(false);
		ui->cbox_DOlvdeng->setChecked(false);
		ui->cbox_DOhongdeng->setChecked(false);

		ui->cbox_DOtifeixinhao->setEnabled(false);
		ui->cbox_DObaojing->setEnabled(false);
		ui->cbox_DOlvdeng->setEnabled(false);
		ui->cbox_DOhongdeng->setEnabled(false);

		monitorZMotionMonitorThread->setRunning(true);
	}
}

void DlgProductSet::cbox_DOtifeixinhao_checked(bool ischecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::tifeixinhaoOut, ischecked);
	}
}

void DlgProductSet::cbox_DObaojing_checked(bool ischecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::baojingOut, ischecked);
	}
}

void DlgProductSet::cbox_DOlvdeng_checked(bool ischecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::lvdengOut, ischecked);
	}
}

void DlgProductSet::cbox_DOhongdeng_checked(bool ischecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::hongdengOut, ischecked);
	}
}

void DlgProductSet::btn_setDIjiaodaitiewan_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_setDIjiaodaitiewan->setText(value);
		setConfig.jiaodaitiewanIN = value.toInt();
		ControlLines::jiaodaitiewanIn = static_cast<int>(value.toDouble());
		auto indicesDI = DIFindAllDuplicateIndices();
		setDIErrorInfo(indicesDI);
		auto indicesDO = DOFindAllDuplicateIndices();
		setDOErrorInfo(indicesDO);
	}
}

void DlgProductSet::btn_setDOtifeixinhao_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_setDOtifeixinhao->setText(value);
		setConfig.tifeixinhaoOUT = value.toInt();
		ControlLines::tifeixinhaoOut = static_cast<int>(value.toDouble());
		auto indicesDO = DOFindAllDuplicateIndices();
		setDOErrorInfo(indicesDO);
		auto indicesDI = DIFindAllDuplicateIndices();
		setDIErrorInfo(indicesDI);
	}
}

void DlgProductSet::btn_setDObaojing_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_setDObaojing->setText(value);
		setConfig.baojingOUT = value.toInt();
		ControlLines::baojingOut = static_cast<int>(value.toDouble());
		auto indicesDO = DOFindAllDuplicateIndices();
		setDOErrorInfo(indicesDO);
		auto indicesDI = DIFindAllDuplicateIndices();
		setDIErrorInfo(indicesDI);
	}
}

void DlgProductSet::btn_setDOlvdeng_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_setDOlvdeng->setText(value);
		setConfig.lvdengOUT = value.toInt();
		ControlLines::lvdengOut = static_cast<int>(value.toDouble());
		auto indicesDO = DOFindAllDuplicateIndices();
		setDOErrorInfo(indicesDO);
		auto indicesDI = DIFindAllDuplicateIndices();
		setDIErrorInfo(indicesDI);
	}
}

void DlgProductSet::btn_setDOhongdeng_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_setDOhongdeng->setText(value);
		setConfig.hongdengOUT = value.toInt();
		ControlLines::hongdengOut = static_cast<int>(value.toDouble());
		auto indicesDO = DOFindAllDuplicateIndices();
		setDOErrorInfo(indicesDO);
		auto indicesDI = DIFindAllDuplicateIndices();
		setDIErrorInfo(indicesDI);
	}
}

void DlgProductSet::cbox_yundongkongzhiqichonglian_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.yundongkongzhiqichonglian = ui->cbox_yundongkongzhiqichonglian->isChecked();
}

void DlgProductSet::btn_xiangjichufachangdu_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_xiangjichufachangdu->setText(value);
		setConfig.xiangjichufachangdu = value.toDouble();
		auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;

		bool isSet = zmotion->setModbus(4, 1, value.toFloat());

		if (!isSet)
		{
			QMessageBox::warning(this, "警告", "设定拉袋长度失败!");
		}
	}
}

void DlgProductSet::btn_meizhuanmaichongshu_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_meizhuanmaichongshu->setText(value);
		setConfig.meizhuanmaichongshu = value.toDouble();
	}
}

void DlgProductSet::btn_shedingzhouchang_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_shedingzhouchang->setText(value);
		setConfig.shedingzhouchang = value.toDouble();
	}
}

void DlgProductSet::btn_tifeijishu_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_tifeijishu->setText(value);
		setConfig.tifeijishu = value.toDouble();
	}
}

void DlgProductSet::btn_liangpinjishu_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_liangpinjishu->setText(value);
		setConfig.liangpinjishu = value.toDouble();
	}
}

void DlgProductSet::btn_baojingchixushijian_clicked()
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
		auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
		ui->btn_baojingchixushijian->setText(value);
		setConfig.baojingchixushijian = value.toDouble();
	}
}

void DlgProductSet::tabWidget_indexChanged(int index)
{
	auto& _isUpdateMonitorInfo = Modules::getInstance().motionControllerModule._isUpdateMonitorInfo;
	auto& monitorZMotionMonitorThread = Modules::getInstance().motionControllerModule.monitorMotionIoStateThread;
	switch (index) {
	case 0:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 1:
		_isUpdateMonitorInfo = true;
		monitorZMotionMonitorThread->setRunning(true);
		break;
	case 2:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	case 3:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	default:
		_isUpdateMonitorInfo = false;
		monitorZMotionMonitorThread->setRunning(false);
		break;
	}
}

void DlgProductSet::btn_wenchongzuidahuiduchazhi_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_wenchongzuidahuiduchazhi->setText(value);
		setConfig.wenchongzuidahuiduchazhi = value.toDouble();
	}
}

void DlgProductSet::btn_wenchongzuixiaohuiduchazhi_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_wenchongzuixiaohuiduchazhi->setText(value);
		setConfig.wenchongzuixiaohuiduchazhi = value.toDouble();
	}
}

void DlgProductSet::btn_wenchongzuidamianji_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_wenchongzuidamianji->setText(value);
		setConfig.wenchongzuidamianji = value.toDouble();
	}
}

void DlgProductSet::btn_wenchongzuixiaomianji_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_wenchongzuixiaomianji->setText(value);
		setConfig.wenchongzuixiaomianji = value.toDouble();
	}
}

void DlgProductSet::btn_maofazuidahuiduchazhi_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_maofazuidahuiduchazhi->setText(value);
		setConfig.maofazuidahuiduchazhi = value.toDouble();
	}
}

void DlgProductSet::btn_maofazuixiaohuiduchazhi_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_maofazuixiaohuiduchazhi->setText(value);
		setConfig.maofazuixiaohuiduchazhi = value.toDouble();
	}
}

void DlgProductSet::btn_maofazuidamianji_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_maofazuidamianji->setText(value);
		setConfig.maofazuidamianji = value.toDouble();
	}
}

void DlgProductSet::btn_maofazuixiaomianji_clicked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
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
		ui->btn_maofazuixiaomianji->setText(value);
		setConfig.maofazuixiaomianji = value.toDouble();
	}
}

void DlgProductSet::monitorInPutSignal(size_t index, bool state)
{
	if (isDebugIO) return;
	for (const auto& item : DICheckItems)
	{
		if (index == *item.line)
		{
			if (item.box)
			{
				QSignalBlocker blocker(item.box);
				item.box->setChecked(state);
			}
			break;
		}
	}
}

void DlgProductSet::monitorOutPutSignal(size_t index, bool state)
{
	if (isDebugIO) return;

	for (const auto& item : DOCheckItems)
	{
		if (index == *item.line)
		{
			if (item.box)
			{
				QSignalBlocker blocker(item.box);
				item.box->setChecked(state);
			}
			break;
		}
	}
}




