#include "DlgProductScore.h"

#include <QMessageBox>
#include <QLabel>
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
}

void DlgProductScore::read_config()
{
	auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;
}

void DlgProductScore::build_connect()
{
	QObject::connect(ui->pbtn_close, &QPushButton::clicked,
		this, &DlgProductScore::pbtn_close_clicked);
}

void DlgProductScore::pbtn_close_clicked()
{
	emit scoreFormClosed();
	this->close();
}

