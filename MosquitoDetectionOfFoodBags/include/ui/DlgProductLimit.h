#pragma once

#include <QDialog>
#include "ui_DlgProductLimit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgProductLimitClass; };
QT_END_NAMESPACE

class DlgProductLimit : public QDialog
{
	Q_OBJECT

public:
	DlgProductLimit(QWidget *parent = nullptr);
	~DlgProductLimit();
public:
	void build_ui();
	void read_config();
	void build_connect();

private slots:
	void pbtn_close_clicked();

public:
	void updateShowImage();
private:
	void getImage();
	void setImage();
	QImage drawLimitLines();
private:
	Ui::DlgProductLimitClass *ui;

	QImage showQImage{};
};

