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

	setAllQLabelStyleSheet();
	setAllQCheckBoxStyleSheet();
	setAllQPushButtonStyleSheet();
}

void DlgProductSet::read_config()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	// 剔废持续时间
	ui->pbtn_tifeichixushijian1->setText(QString::number(setConfig.tifeichixushijian1));
	ui->pbtn_tifeichixushijian2->setText(QString::number(setConfig.tifeichixushijian2));
	// 剔废时间
	ui->pbtn_tifeijuli1->setText(QString::number(setConfig.tifeijuli1));
	ui->pbtn_tifeijuli2->setText(QString::number(setConfig.tifeijuli2));

	// 采图
	ui->cBox_takeCamera1Pictures->setChecked(setConfig.takeWork1Pictures);
	ui->cBox_takeCamera2Pictures->setChecked(setConfig.takeWork2Pictures);

	// 存图
	ui->cBox_takeNgPictures->setChecked(setConfig.saveNGImg);
	ui->cBox_takeMaskPictures->setChecked(setConfig.saveMaskImg);
	ui->cBox_takeOkPictures->setChecked(setConfig.saveOKImg);

	// 一工位
	ui->pbtn_shangxianwei1->setText(QString::number(setConfig.shangXianWei1));
	ui->pbtn_xiaxianwei1->setText(QString::number(setConfig.xiaXianWei1));
	ui->pbtn_zuoxianwei1->setText(QString::number(setConfig.zuoXianWei1));
	ui->pbtn_youxianwei1->setText(QString::number(setConfig.youXianWei1));
	ui->pbtn_baoguang1->setText(QString::number(setConfig.baoguang1));
	ui->pbtn_zengyi1->setText(QString::number(setConfig.zengyi1));
	ui->pbtn_xiangsudangliang1->setText(QString::number(setConfig.xiangSuDangLiang1));

	// 二工位
	ui->pbtn_shangxianwei2->setText(QString::number(setConfig.shangXianWei2));
	ui->pbtn_xiaxianwei2->setText(QString::number(setConfig.xiaXianWei2));
	ui->pbtn_zuoxianwei2->setText(QString::number(setConfig.zuoXianWei2));
	ui->pbtn_youxianwei2->setText(QString::number(setConfig.youXianWei2));
	ui->pbtn_baoguang2->setText(QString::number(setConfig.baoguang2));
	ui->pbtn_zengyi2->setText(QString::number(setConfig.zengyi2));
	ui->pbtn_xiangsudangliang2->setText(QString::number(setConfig.xiangSuDangLiang2));

	// 调试模式默认为关闭
	setConfig.debugMode = false;
	ui->cbox_debugMode->setChecked(setConfig.debugMode);

	// 基本功能
	ui->cbox_qiyongerxiangji->setChecked(setConfig.qiyongerxiangji);
	ui->cbox_qiyongyundongkongzhiqi->setChecked(setConfig.qiyongyundongkongzhiqi);
	ui->cbox_yundongkongzhiqichonglian->setChecked(setConfig.yundongkongzhiqichonglian);

	// 基本参数
	ui->btn_xiangjichufachangdu->setText(QString::number(setConfig.xiangjichufachangdu));
	ui->btn_meizhuanmaichongshu->setText(QString::number(setConfig.meizhuanmaichongshu));
	ui->btn_shedingzhouchang->setText(QString::number(setConfig.shedingzhouchang));

	// 设置IO
	ui->btn_setDOtifeixinhao->setText(QString::number(setConfig.tifeixinhaoOUT));
	ui->btn_setDOxiangjichufa1->setText(QString::number(setConfig.xiangjichufa1OUT));
	ui->btn_setDOxiangjichufa2->setText(QString::number(setConfig.xiangjichufa2OUT));

	// 分数界面可选显示
	ui->ckb_zangwu->setChecked(setConfig.isZangWu);
	ui->ckb_xiaozangwu->setChecked(setConfig.isXiaoZangWu);
	ui->ckb_jietou->setChecked(setConfig.isJieTou);
	ui->ckb_body->setChecked(setConfig.isBody);
	ui->ckb_huapo->setChecked(setConfig.isHuaPo);
	ui->ckb_weizhiquexian->setChecked(setConfig.isWeiZhiQueXian);

	//未知缺陷
	ui->btn_defectIgnoreX->setText(QString::number(setConfig.defectIgnoreX));
	ui->btn_defectIgnoreY->setText(QString::number(setConfig.defectIgnoreY));

	// 默认显示第一个
	ui->tabWidget->setCurrentIndex(0);

	ui->cbox_isTifeidongzuo->setChecked(setConfig.isTichuDongzuo);

	updateControlLines();

	initDOCheckItems();
	initDICheckItems();
}

void DlgProductSet::setAllQLabelStyleSheet()
{
	static constexpr const char* kLabelStyle = R"(QLabel {
        color: #444444;
        font-size: 20px;
        background: transparent;
        padding: 2px 4px;
    })";

	const auto labels = findChildren<QLabel*>();
	for (QLabel* label : labels)
	{
		if (!label) continue;
		label->setStyleSheet(kLabelStyle);
	}
}

void DlgProductSet::setAllQCheckBoxStyleSheet()
{
	static constexpr const char* kCheckBoxStyle = R"(QCheckBox {
        spacing: 8px;
        font-size: 20px;
        font-weight: bold;
        color: rgb(0, 0, 0);
    }
    QCheckBox::indicator {
        width: 20px;
        height: 20px;
        border-radius: 4px;
        border: 2px solid #999;
        background: #ffffff;
    }
    QCheckBox::indicator:hover {
        border: 2px solid #666;
    }
    QCheckBox::indicator:checked {
        border: 2px solid #2196F3;
        background-color: qradialgradient(
            cx:0.5, cy:0.5,
            radius:0.4,
            fx:0.5, fy:0.5,
            stop:0 #2196F3,
            stop:1 transparent
        );
    }
    QCheckBox::indicator:pressed {
        border: 2px solid #1565C0;
        background-color: qradialgradient(
            cx:0.5, cy:0.5,
            radius:0.5,
            fx:0.5, fy:0.5,
            stop:0 #BBDEFB,
            stop:1 transparent
        );
    }
    QCheckBox::indicator:disabled {
        border: 2px solid #ccc;
        background: #eee;
    }
    QCheckBox:checked {
        color: #2196F3;
        font-weight: 500;
    })";

	const auto checkBoxes = findChildren<QCheckBox*>();
	for (QCheckBox* cb : checkBoxes)
	{
		if (!cb) continue;
		cb->setStyleSheet(kCheckBoxStyle);
	}
}

void DlgProductSet::setAllQPushButtonStyleSheet()
{
	static constexpr const char* kPushButtonStyle = R"(QPushButton {
        padding: 6px 14px;
        border: 2px solid #CCC;
        border-radius: 4px;
        background-color: white;
        color: #444;
        font-size: 20px;
    }
    QPushButton:hover {
        border-color: #999;
        background-color: #F5F5F5;
    }
    QPushButton:pressed {
        border-color: #777;
        background-color: #EEE;
    })";

	const auto pushButtons = findChildren<QPushButton*>();
	for (QPushButton* pb : pushButtons)
	{
		if (!pb) continue;
		pb->setStyleSheet(kPushButtonStyle);
	}
}

void DlgProductSet::build_connect()
{
	QObject::connect(ui->pbtn_tifeichixushijian1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_tifeichixushijian1_clicked);
	QObject::connect(ui->pbtn_tifeichixushijian2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_tifeichixushijian2_clicked);
	QObject::connect(ui->pbtn_tifeijuli1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_tifeijuli1_clicked);
	QObject::connect(ui->pbtn_tifeijuli2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_tifeijuli2_clicked);
	QObject::connect(ui->pbtn_shangxianwei1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_shangxianwei1_clicked);
	QObject::connect(ui->pbtn_xiaxianwei1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_xiaxianwei1_clicked);
	QObject::connect(ui->pbtn_zuoxianwei1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_zuoxianwei1_clicked);
	QObject::connect(ui->pbtn_youxianwei1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_youxianwei1_clicked);
	QObject::connect(ui->pbtn_baoguang1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_baoguang1_clicked);
	QObject::connect(ui->pbtn_zengyi1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_zengyi1_clicked);
	QObject::connect(ui->pbtn_xiangsudangliang1, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_xiangsudangliang1_clicked);
	QObject::connect(ui->pbtn_shangxianwei2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_shangxianwei2_clicked);
	QObject::connect(ui->pbtn_xiaxianwei2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_xiaxianwei2_clicked);
	QObject::connect(ui->pbtn_zuoxianwei2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_zuoxianwei2_clicked);
	QObject::connect(ui->pbtn_youxianwei2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_youxianwei2_clicked);
	QObject::connect(ui->pbtn_baoguang2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_baoguang2_clicked);
	QObject::connect(ui->pbtn_zengyi2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_zengyi2_clicked);
	QObject::connect(ui->pbtn_xiangsudangliang2, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_xiangsudangliang2_clicked);
	QObject::connect(ui->cBox_takeNgPictures, &QCheckBox::clicked,
		this, &DlgProductSet::cBox_takeNgPictures_checked);
	QObject::connect(ui->cBox_takeMaskPictures, &QCheckBox::clicked,
		this, &DlgProductSet::cBox_takeMaskPictures_checked);
	QObject::connect(ui->cBox_takeOkPictures, &QCheckBox::clicked,
		this, &DlgProductSet::cBox_takeOkPictures_checked);
	QObject::connect(ui->cbox_debugMode, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_debugMode_checked);
	QObject::connect(ui->pbtn_close, &QPushButton::clicked,
		this, &DlgProductSet::pbtn_close_clicked);
	QObject::connect(ui->cBox_takeCamera1Pictures, &QCheckBox::clicked,
		this, &DlgProductSet::cBox_takeCamera1Pictures_checked);
	QObject::connect(ui->cBox_takeCamera2Pictures, &QCheckBox::clicked,
		this, &DlgProductSet::cBox_takeCamera2Pictures_checked);
	QObject::connect(ui->cbox_qiyongerxiangji, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_qiyongerxiangji_checked);
	QObject::connect(ui->cbox_qiyongyundongkongzhiqi, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_qiyongyundongkongzhiqi_checked);
	QObject::connect(ui->cbox_yundongkongzhiqichonglian, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_yundongkongzhiqichonglian_checked);
	QObject::connect(ui->btn_defectIgnoreX, &QPushButton::clicked,
		this, &DlgProductSet::btn_defectIgnoreX_clicked);
	QObject::connect(ui->btn_defectIgnoreY, &QPushButton::clicked,
		this, &DlgProductSet::btn_defectIgnoreY_clicked);

	// 基本参数
	QObject::connect(ui->btn_xiangjichufachangdu, &QPushButton::clicked,
		this, &DlgProductSet::btn_xiangjichufachangdu_clicked);
	QObject::connect(ui->btn_meizhuanmaichongshu, &QPushButton::clicked,
		this, &DlgProductSet::btn_meizhuanmaichongshu_clicked);
	QObject::connect(ui->btn_shedingzhouchang, &QPushButton::clicked,
		this, &DlgProductSet::btn_shedingzhouchang_clicked);

	// 监控IO
	QObject::connect(ui->tabWidget, &QTabWidget::currentChanged,
		this, &DlgProductSet::tabWidget_indexChanged);
	QObject::connect(ui->cbox_DOtifeixinhao, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_DOtifeixinhao_checked);
	QObject::connect(ui->cbox_DOxiangjichufa1, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_DOxiangjichufa1_checked);
	QObject::connect(ui->cbox_DOxiangjichufa2, &QCheckBox::clicked,
		this, &DlgProductSet::cbox_DOxiangjichufa2_checked);

	// 设置IO
	QObject::connect(ui->btn_setDOtifeixinhao, &QPushButton::clicked,
		this, &DlgProductSet::btn_setDOtifeixinhao_clicked);
	QObject::connect(ui->btn_setDOxiangjichufa1, &QPushButton::clicked,
		this, &DlgProductSet::btn_setDOxiangjichufa1_clicked);
	QObject::connect(ui->btn_setDOxiangjichufa2, &QPushButton::clicked,
		this, &DlgProductSet::btn_setDOxiangjichufa2_clicked);

	// 分数界面可选显示
	QObject::connect(ui->ckb_zangwu, &QCheckBox::clicked,
		this, &DlgProductSet::ckb_zangwu_checked);
	QObject::connect(ui->ckb_xiaozangwu, &QCheckBox::clicked,
		this, &DlgProductSet::ckb_xiaozangwu_checked);
	QObject::connect(ui->ckb_jietou, &QCheckBox::clicked,
		this, &DlgProductSet::ckb_jietou_checked);
	QObject::connect(ui->ckb_body, &QCheckBox::clicked,
		this, &DlgProductSet::ckb_body_checked);
	QObject::connect(ui->ckb_huapo, &QCheckBox::clicked,
		this, &DlgProductSet::ckb_huapo_checked);
	QObject::connect(ui->ckb_weizhiquexian, &QCheckBox::clicked,
		this, &DlgProductSet::ckb_weizhiquexian_checked);

	QObject::connect(ui->cbox_isTifeidongzuo, &QCheckBox::clicked,
		this, &DlgProductSet::ckb_zangwu_checked);
}

void DlgProductSet::setDIErrorInfo(const std::vector<std::vector<int>>& index)
{
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
	default:
		break;
	}
}

void DlgProductSet::updateControlLines()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	ControlLines::tifeixinhaoOut = setConfig.tifeixinhaoOUT;
	ControlLines::xiangjichufa1Out = setConfig.xiangjichufa1OUT;
	ControlLines::xiangjichufa2Out = setConfig.xiangjichufa2OUT;
}

void DlgProductSet::initDOCheckItems()
{
	DOCheckItems.clear();
	DOCheckItems = {
		{ &ControlLines::tifeixinhaoOut, ui->cbox_DOtifeixinhao },
		{ &ControlLines::xiangjichufa1Out, ui->cbox_DOxiangjichufa1 },
		{ &ControlLines::xiangjichufa2Out, ui->cbox_DOxiangjichufa2 }
	};
}

void DlgProductSet::initDICheckItems()
{
	DICheckItems.clear();
	DICheckItems = {
		
	};
}

std::vector<std::vector<int>> DlgProductSet::DOFindAllDuplicateIndices()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	std::vector<int> values = {
		setConfig.tifeixinhaoOUT,
		setConfig.xiangjichufa1OUT,
		setConfig.xiangjichufa2OUT
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
	ui->lb_DOxiangjichufa1->clear();
	ui->lb_DOxiangjichufa2->clear();

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
		ui->lb_DOxiangjichufa1->setText(text);
		break;
	case 2:
		ui->lb_DOxiangjichufa2->setText(text);
		break;
	default:
		break;
	}
}

std::vector<std::vector<int>> DlgProductSet::DIFindAllDuplicateIndices()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	std::vector<int> values = {

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

void DlgProductSet::pbtn_tifeichixushijian1_clicked()
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
		ui->pbtn_tifeichixushijian1->setText(value);
		setConfig.tifeichixushijian1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_tifeichixushijian2_clicked()
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
		ui->pbtn_tifeichixushijian2->setText(value);
		setConfig.tifeichixushijian2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_tifeijuli1_clicked()
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
		ui->pbtn_tifeijuli1->setText(value);
		setConfig.tifeijuli1 = value.toDouble();
		emit tifeijuliChanged();
	}
}

void DlgProductSet::pbtn_tifeijuli2_clicked()
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
		ui->pbtn_tifeijuli2->setText(value);
		setConfig.tifeijuli2 = value.toDouble();
		emit tifeijuliChanged();
	}
}

void DlgProductSet::pbtn_shangxianwei1_clicked()
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
		ui->pbtn_shangxianwei1->setText(value);
		setConfig.shangXianWei1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_xiaxianwei1_clicked()
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
		ui->pbtn_xiaxianwei1->setText(value);
		setConfig.xiaXianWei1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_zuoxianwei1_clicked()
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
		ui->pbtn_zuoxianwei1->setText(value);
		setConfig.zuoXianWei1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_youxianwei1_clicked()
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
		ui->pbtn_youxianwei1->setText(value);
		setConfig.youXianWei1 = value.toDouble();
	}
}

void DlgProductSet::pbtn_baoguang1_clicked()
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
		ui->pbtn_baoguang1->setText(value);
		setConfig.baoguang1 = value.toDouble();
		if (camera1)
		{
			camera1->setExposureTime(static_cast<size_t>(value.toDouble()));
		}
	}
}

void DlgProductSet::pbtn_zengyi1_clicked()
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
		ui->pbtn_zengyi1->setText(value);
		setConfig.zengyi1 = value.toDouble();
		if (camera1)
		{
			camera1->setGain(static_cast<size_t>(value.toDouble()));
		}
	}
}

void DlgProductSet::pbtn_xiangsudangliang1_clicked()
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
		ui->pbtn_xiangsudangliang1->setText(value);
		setConfig.xiangSuDangLiang1 = value.toDouble();
		emit pixToWorldChanged();
	}
}

void DlgProductSet::pbtn_shangxianwei2_clicked()
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
		ui->pbtn_shangxianwei2->setText(value);
		setConfig.shangXianWei2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_xiaxianwei2_clicked()
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
		ui->pbtn_xiaxianwei2->setText(value);
		setConfig.xiaXianWei2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_zuoxianwei2_clicked()
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
		ui->pbtn_zuoxianwei2->setText(value);
		setConfig.zuoXianWei2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_youxianwei2_clicked()
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
		ui->pbtn_youxianwei2->setText(value);
		setConfig.youXianWei2 = value.toDouble();
	}
}

void DlgProductSet::pbtn_baoguang2_clicked()
{
	/*NumberKeyboard numKeyBord;
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
		auto& camera2 = Modules::getInstance().cameraModule.camera2;
		ui->pbtn_baoguang2->setText(value);
		setConfig.baoguang2 = value.toDouble();
		if (camera2)
		{
			camera2->setExposureTime(static_cast<size_t>(value.toDouble()));
		}
	}*/
}

void DlgProductSet::pbtn_zengyi2_clicked()
{
	/*NumberKeyboard numKeyBord;
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
		auto& camera2 = Modules::getInstance().cameraModule.camera2;
		ui->pbtn_zengyi2->setText(value);
		setConfig.zengyi2 = value.toDouble();
		if (camera2)
		{
			camera2->setGain(static_cast<size_t>(value.toDouble()));
		}
	}*/
}

void DlgProductSet::pbtn_xiangsudangliang2_clicked()
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
		ui->pbtn_xiangsudangliang2->setText(value);
		setConfig.xiangSuDangLiang2 = value.toDouble();
		emit pixToWorldChanged();
	}
}

void DlgProductSet::btn_defectIgnoreX_clicked()
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
		ui->btn_defectIgnoreX->setText(value);
		setConfig.defectIgnoreX = value.toDouble();
	}
}

void DlgProductSet::btn_defectIgnoreY_clicked()
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
		ui->btn_defectIgnoreY->setText(value);
		setConfig.defectIgnoreY = value.toDouble();
	}
}

void DlgProductSet::cBox_takeNgPictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.saveNGImg = ui->cBox_takeNgPictures->isChecked();
}

void DlgProductSet::cBox_takeMaskPictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.saveMaskImg = ui->cBox_takeMaskPictures->isChecked();
}

void DlgProductSet::cBox_takeOkPictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.saveOKImg = ui->cBox_takeOkPictures->isChecked();
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
		ui->cbox_DOxiangjichufa1->setChecked(false);
		ui->cbox_DOxiangjichufa2->setChecked(false);

		ui->cbox_DOtifeixinhao->setEnabled(true);
		ui->cbox_DOxiangjichufa1->setEnabled(true);
		ui->cbox_DOxiangjichufa2->setEnabled(true);

		monitorZMotionMonitorThread->setRunning(false);
	}
	else
	{
		ui->cbox_DOtifeixinhao->setChecked(false);
		ui->cbox_DOxiangjichufa1->setChecked(false);
		ui->cbox_DOxiangjichufa2->setChecked(false);

		ui->cbox_DOtifeixinhao->setEnabled(false);
		ui->cbox_DOxiangjichufa1->setEnabled(false);
		ui->cbox_DOxiangjichufa2->setEnabled(false);

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

void DlgProductSet::cbox_DOxiangjichufa1_checked(bool ischecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::xiangjichufa1Out, ischecked);
	}
}

void DlgProductSet::cbox_DOxiangjichufa2_checked(bool ischecked)
{
	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	if (isDebugIO)
	{
		auto isSuccess = zmotion->setIOOut(ControlLines::xiangjichufa2Out, ischecked);
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
		setConfig.tifeixinhaoOUT = value.toDouble();
		ControlLines::tifeixinhaoOut = static_cast<int>(value.toDouble());
		auto indicesDO = DOFindAllDuplicateIndices();
		setDOErrorInfo(indicesDO);
		auto indicesDI = DIFindAllDuplicateIndices();
		setDIErrorInfo(indicesDI);
	}
}

void DlgProductSet::btn_setDOxiangjichufa1_clicked()
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
		ui->btn_setDOxiangjichufa1->setText(value);
		setConfig.xiangjichufa1OUT = value.toDouble();
		ControlLines::xiangjichufa1Out = static_cast<int>(value.toDouble());
		auto indicesDO = DOFindAllDuplicateIndices();
		setDOErrorInfo(indicesDO);
		auto indicesDI = DIFindAllDuplicateIndices();
		setDIErrorInfo(indicesDI);
	}
}

void DlgProductSet::btn_setDOxiangjichufa2_clicked()
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
		ui->btn_setDOxiangjichufa2->setText(value);
		setConfig.xiangjichufa2OUT = value.toDouble();
		ControlLines::xiangjichufa2Out = static_cast<int>(value.toDouble());
		auto indicesDO = DOFindAllDuplicateIndices();
		setDOErrorInfo(indicesDO);
		auto indicesDI = DIFindAllDuplicateIndices();
		setDIErrorInfo(indicesDI);
	}
}

void DlgProductSet::ckb_zangwu_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.isTichuDongzuo = isChecked;
}

void DlgProductSet::ckb_xiaozangwu_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.isXiaoZangWu = isChecked;
}

void DlgProductSet::ckb_jietou_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.isJieTou = isChecked;
}

void DlgProductSet::ckb_body_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.isBody = isChecked;
}

void DlgProductSet::ckb_huapo_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.isHuaPo = isChecked;
}

void DlgProductSet::ckb_weizhiquexian_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.isWeiZhiQueXian = isChecked;
}

void DlgProductSet::cbox_isTifeidongzuo_checked(bool isChecked)
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.isTichuDongzuo = isChecked;
}

void DlgProductSet::cBox_takeCamera1Pictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.takeWork1Pictures = ui->cBox_takeCamera1Pictures->isChecked();
}

void DlgProductSet::cBox_takeCamera2Pictures_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.takeWork2Pictures = ui->cBox_takeCamera2Pictures->isChecked();
}

void DlgProductSet::cbox_qiyongerxiangji_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.qiyongerxiangji = ui->cbox_qiyongerxiangji->isChecked();
}

void DlgProductSet::cbox_qiyongyundongkongzhiqi_checked()
{
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	setConfig.qiyongyundongkongzhiqi = ui->cbox_qiyongyundongkongzhiqi->isChecked();
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




