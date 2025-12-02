#include "DlgProductLimit.h"

#include <QPainter>

#include "MDOFoodBags.h"
#include "rqw_HalconUtilty.hpp"
#include "Modules.hpp"

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

void DlgProductLimit::showEvent(QShowEvent* event)
{
	QDialog::showEvent(event);

	if (event->spontaneous()) {
		return;
	}

	updateShowImage();
}

void DlgProductLimit::pbtn_close_clicked()
{
	this->close();
}

void DlgProductLimit::updateShowImage()
{
	getImage();
	setImage();
}

void DlgProductLimit::getImage()
{
	if (MDOFoodBags::getIsModelImageLoaded())
	{
		auto showImagePtr = MDOFoodBags::getModelQImage();
		auto showImage = *showImagePtr;
		showQImage = showImage;
	}
}

void DlgProductLimit::setImage()
{
	auto tempQImage = drawLimitLines();
	auto showImage = QPixmap::fromImage(tempQImage);
	ui->label_imgDisplay->setPixmap(showImage.scaled(ui->label_imgDisplay->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QImage DlgProductLimit::drawLimitLines()
{
	auto setConfig = Modules::getInstance().configManagerModule.setConfig;
	auto leftLimit = setConfig.zuoXianWei;
	auto rightLimit = setConfig.youXianWei;

	// 绘制限位
	QImage tempQImage = showQImage;
	// 左限位
	drawVerticalLimitLine(tempQImage, leftLimit);
	// 右限位
	drawVerticalLimitLine(tempQImage, rightLimit);

	return tempQImage;
}

void DlgProductLimit::drawVerticalLimitLine(QImage& image, int position)
{
	QPainter painter(&image);

	// 设置黄色虚线，线宽为2
	QPen pen(Qt::yellow, 2, Qt::DashLine);
	painter.setPen(pen);

	// 绘制从顶部到底部的垂直线
	painter.drawLine(position, 0, position, image.height());
}
