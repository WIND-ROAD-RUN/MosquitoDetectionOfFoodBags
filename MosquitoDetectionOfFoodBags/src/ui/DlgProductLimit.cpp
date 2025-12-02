#include "DlgProductLimit.h"
#include "MDOFoodBags.h"
#include "rqw_HalconUtilty.hpp"

DlgProductLimit::DlgProductLimit(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgProductLimitClass())
{
	ui->setupUi(this);

	build_ui();

	build_connect();
}

DlgProductLimit::~DlgProductLimit()
{
	delete ui;
}

void DlgProductLimit::build_ui()
{
	read_config();
}

void DlgProductLimit::read_config()
{

}

void DlgProductLimit::build_connect()
{
	connect(ui->btn_close, &QPushButton::clicked, this, &DlgProductLimit::pbtn_close_clicked);
}

void DlgProductLimit::pbtn_close_clicked()
{
	this->close();
}

void DlgProductLimit::getImage()
{
	
}

