#pragma once

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class DlgProductScoreClass; };
QT_END_NAMESPACE

class DlgProductScore : public QDialog
{
	Q_OBJECT

public:
	DlgProductScore(QWidget* parent = nullptr);
	~DlgProductScore();

public:
	void build_ui();
	void read_config();
	void build_connect();

private slots:
	void pbtn_close_clicked();

signals:
	void scoreFormClosed();

public:
	Ui::DlgProductScoreClass* ui;
};

