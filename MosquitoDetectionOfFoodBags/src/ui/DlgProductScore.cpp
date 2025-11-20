#include "DlgProductScore.h"

#include <QMessageBox>
#include <QLabel>
#include "GlobalStruct.hpp"
#include "Modules.hpp"
#include "NumberKeyboard.h"
#include "ui_DlgProductScore.h"

DlgProductScore::DlgProductScore(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgProductScoreClass())
{
	ui->setupUi(this);

	build_ui();

	build_connect();
}

DlgProductScore::~DlgProductScore()
{
	delete ui;
}

void DlgProductScore::build_ui()
{
	read_config();
	setAllQLabelStyleSheet();
	setAllQRadioButtonStyleSheet();
	setAllQPushButtonStyleSheet();
}

void DlgProductScore::read_config()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	// 初始化参数

	// 脏污
	ui->rbtn_zangwuEnable->setChecked(scoreConfig.zangWu);
	ui->ptn_zangwuSimilarity->setText(QString::number(scoreConfig.zangWuScore));
	ui->ptn_zangwuArea->setText(QString::number(scoreConfig.zangWuArea));

	// 接头
	ui->rbtn_jietouEnable->setChecked(scoreConfig.jieTou);
	ui->pbtn_jietouSimilarity->setText(QString::number(scoreConfig.jieTouScore));
	ui->pbtn_jietouArea->setText(QString::number(scoreConfig.jieTouArea));

	// 小脏污
	ui->rbtn_xiaozangwuEnable->setChecked(scoreConfig.xiaoZangWu);
	ui->pbtn_xiaozangwuSimilarity->setText(QString::number(scoreConfig.xiaoZangWuScore));
	ui->pbtn_xiaozangwuArea->setText(QString::number(scoreConfig.xiaoZangWuArea));

	// 本体
	ui->rbtn_bodyEnable->setChecked(scoreConfig.body);
	ui->pbtn_bodySimilarity->setText(QString::number(scoreConfig.bodyScore));
	ui->pbtn_bodyArea->setText(QString::number(scoreConfig.bodyArea));

	// 划破
	ui->rbtn_huapoEnable->setChecked(scoreConfig.huaPo);
	ui->pbtn_huapoSimilarity->setText(QString::number(scoreConfig.huaPoScore));
	ui->pbtn_huapoArea->setText(QString::number(scoreConfig.huaPoArea));

	// 未知缺陷
	ui->rbtn_weizhiquexianSingleSimilarityEnable->setChecked(scoreConfig.weiZhiQueXianSingle);
	ui->pbtn_weizhiquexianSingleSimilarity->setText(QString::number(scoreConfig.weiZhiQueXianSingleArea));
	ui->rbtn_weizhiquexianAllAreaEnable->setChecked(scoreConfig.weiZhiQueXianAll);
	ui->pbtn_weizhiquexianAllArea->setText(QString::number(scoreConfig.weiZhiQueXianAllArea));
}

void DlgProductScore::setAllQLabelStyleSheet()
{
	static constexpr const char* kLabelStyle = R"(QLabel {
        color: #444444;
        font-size: 24px;
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

void DlgProductScore::setAllQRadioButtonStyleSheet()
{
	static constexpr const char* kRadioStyle = R"(QRadioButton {
        spacing: 8px;
        font-size: 24px;
        color: #333;
    }
    QRadioButton::indicator {
        width: 20px;
        height: 20px;
        border-radius: 10px;
        border: 2px solid #999;
        background: #ffffff;
    }
    QRadioButton::indicator:hover {
        border: 2px solid #666;
    }
    QRadioButton::indicator:checked {
        border: 2px solid #FF6B00;
        background-color: qradialgradient(
            cx:0.5, cy:0.5,
            radius:0.4,
            fx:0.5, fy:0.5,
            stop:0 #FF6B00,
            stop:1 transparent
        );
    }
    QRadioButton::indicator:pressed {
        border: 2px solid #CC5500;
        background-color: qradialgradient(
            cx:0.5, cy:0.5,
            radius:0.5,
            fx:0.5, fy:0.5,
            stop:0 #FFD8B3,
            stop:1 transparent
        );
    }
    QRadioButton::indicator:disabled {
        border: 2px solid #ccc;
        background: #eee;
    })";

	const auto radios = findChildren<QRadioButton*>();
	for (QRadioButton* rb : radios)
	{
		if (!rb) continue;
		rb->setStyleSheet(kRadioStyle);
	}
}

void DlgProductScore::setAllQPushButtonStyleSheet()
{
	static constexpr const char* kPushButtonStyle = R"(QPushButton {
        padding: 6px 14px;
        border: 2px solid #CCC;
        border-radius: 4px;
        background-color: white;
        color: #444;
        font-size: 24px;
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

void DlgProductScore::build_connect()
{
	QObject::connect(ui->pbtn_close, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_close_clicked);
	QObject::connect(ui->rbtn_zangwuEnable, &QRadioButton::clicked,
		this, &DlgProductScore::rbtn_zangwuEnable_checked);
	QObject::connect(ui->ptn_zangwuSimilarity, &QPushButton::clicked,
		this, &DlgProductScore::ptn_zangwuSimilarity_clicked);
	QObject::connect(ui->ptn_zangwuArea, &QPushButton::clicked,
		this, &DlgProductScore::ptn_zangwuArea_clicked);
	QObject::connect(ui->rbtn_jietouEnable, &QRadioButton::clicked,
		this, &DlgProductScore::rbtn_jietouEnable_checked);
	QObject::connect(ui->pbtn_jietouSimilarity, &QPushButton::clicked,
		this, &DlgProductScore::ptn_jietouSimilarity_clicked);
	QObject::connect(ui->pbtn_jietouArea, &QPushButton::clicked,
		this, &DlgProductScore::ptn_jietouArea_clicked);
	QObject::connect(ui->rbtn_xiaozangwuEnable, &QRadioButton::clicked,
		this, &DlgProductScore::rbtn_xiaozangwuEnable_checked);
	QObject::connect(ui->pbtn_xiaozangwuSimilarity, &QPushButton::clicked,
		this, &DlgProductScore::ptn_xiaozangwuSimilarity_clicked);
	QObject::connect(ui->pbtn_xiaozangwuArea, &QPushButton::clicked,
		this, &DlgProductScore::ptn_xiaozangwuArea_clicked);
	QObject::connect(ui->rbtn_bodyEnable, &QRadioButton::clicked,
		this, &DlgProductScore::rbtn_bodyEnable_checked);
	QObject::connect(ui->pbtn_bodySimilarity, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_bodySimilarity_clicked);
	QObject::connect(ui->pbtn_bodyArea, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_bodyArea_clicked);
	QObject::connect(ui->rbtn_huapoEnable, &QRadioButton::clicked,
		this, &DlgProductScore::rbtn_huapoEnable_checked);
	QObject::connect(ui->pbtn_huapoSimilarity, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_huapoSimilarity_clicked);
	QObject::connect(ui->pbtn_huapoArea, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_huapoArea_clicked);
	QObject::connect(ui->rbtn_weizhiquexianSingleSimilarityEnable, &QRadioButton::toggled,
		this, &DlgProductScore::rbtn_weizhiquexianSingleSimilarityEnable_checked);
	QObject::connect(ui->pbtn_weizhiquexianSingleSimilarity, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_weizhiquexianSingleSimilarity_clicked);
	QObject::connect(ui->rbtn_weizhiquexianAllAreaEnable, &QRadioButton::toggled,
		this, &DlgProductScore::rbtn_weizhiquexianAllAreaEnable_checked);
	QObject::connect(ui->pbtn_weizhiquexianAllArea, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_weizhiquexianAllArea_clicked);
}

void DlgProductScore::pbtn_close_clicked()
{
	emit scoreFormClosed();
	this->close();
}

void DlgProductScore::rbtn_zangwuEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.zangWu = ui->rbtn_zangwuEnable->isChecked();
}

void DlgProductScore::ptn_zangwuSimilarity_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->ptn_zangwuSimilarity->setText(value);
		scoreConfig.zangWuScore = value.toDouble();
	}
}

void DlgProductScore::ptn_zangwuArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->ptn_zangwuArea->setText(value);
		scoreConfig.zangWuArea = value.toDouble();
	}
}

void DlgProductScore::rbtn_jietouEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.jieTou = ui->rbtn_jietouEnable->isChecked();
}

void DlgProductScore::ptn_jietouSimilarity_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_jietouSimilarity->setText(value);
		scoreConfig.jieTouScore = value.toDouble();
	}
}

void DlgProductScore::ptn_jietouArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_jietouArea->setText(value);
		scoreConfig.jieTouArea = value.toDouble();
	}
}

void DlgProductScore::rbtn_xiaozangwuEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.xiaoZangWu = ui->rbtn_xiaozangwuEnable->isChecked();
}

void DlgProductScore::ptn_xiaozangwuSimilarity_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_xiaozangwuSimilarity->setText(value);
		scoreConfig.xiaoZangWuScore = value.toDouble();
	}
}

void DlgProductScore::ptn_xiaozangwuArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_xiaozangwuArea->setText(value);
		scoreConfig.xiaoZangWuArea = value.toDouble();
	}
}

void DlgProductScore::rbtn_bodyEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.body = ui->rbtn_bodyEnable->isChecked();
}

void DlgProductScore::pbtn_bodySimilarity_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_bodySimilarity->setText(value);
		scoreConfig.bodyScore = value.toDouble();
	}
}

void DlgProductScore::pbtn_bodyArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_bodyArea->setText(value);
		scoreConfig.bodyArea = value.toDouble();
	}
}

void DlgProductScore::rbtn_huapoEnable_checked()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.huaPo = ui->rbtn_huapoEnable->isChecked();
}

void DlgProductScore::pbtn_huapoSimilarity_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_huapoSimilarity->setText(value);
		scoreConfig.huaPoScore = value.toDouble();
	}
}

void DlgProductScore::pbtn_huapoArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_huapoArea->setText(value);
		scoreConfig.huaPoArea = value.toDouble();
	}
}

void DlgProductScore::rbtn_weizhiquexianSingleSimilarityEnable_checked(bool isChecked)
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.weiZhiQueXianSingle = isChecked;
}

void DlgProductScore::pbtn_weizhiquexianSingleSimilarity_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_weizhiquexianSingleSimilarity->setText(value);
		scoreConfig.weiZhiQueXianSingleArea = value.toDouble();
	}
}

void DlgProductScore::rbtn_weizhiquexianAllAreaEnable_checked(bool isChecked)
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
	scoreConfig.weiZhiQueXianAll = isChecked;
}

void DlgProductScore::pbtn_weizhiquexianAllArea_clicked()
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
		auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
		ui->pbtn_weizhiquexianAllArea->setText(value);
		scoreConfig.weiZhiQueXianAllArea = value.toDouble();
	}
}


