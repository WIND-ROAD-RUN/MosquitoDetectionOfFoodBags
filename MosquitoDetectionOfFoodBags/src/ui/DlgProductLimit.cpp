#include "DlgProductLimit.h"
#include "MDOFoodBags.h"
#include "rqw_HalconUtilty.hpp"
#include "Modules.hpp"
#include "../../../ThirdRep/RW_UL/RW_UL/Module/ImgPro/include/imgPro_ImagePainter.hpp"
#include "../../../ThirdRep/RW_UL/RW_UL/Module/ImgPro/include/imgPro_ImageProcessUtilty.hpp"

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

void DlgProductLimit::updateShowImage()
{
	getImage();
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
	rw::imgPro::ConfigDrawLine cfg;
	cfg.position = leftLimit;
	rw::imgPro::ImagePainter::drawVerticalLine(tempQImage, cfg);
	// 右限位
	cfg.position = rightLimit;
	rw::imgPro::ImagePainter::drawVerticalLine(tempQImage, cfg);

	return tempQImage;
}

