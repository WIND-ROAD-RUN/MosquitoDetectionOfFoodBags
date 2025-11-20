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
	void setAllQLabelStyleSheet();
	void setAllQRadioButtonStyleSheet();
	void setAllQPushButtonStyleSheet();
	void build_connect();

private slots:
	void pbtn_close_clicked();

	void rbtn_zangwuEnable_checked();
	void ptn_zangwuSimilarity_clicked();
	void ptn_zangwuArea_clicked();
	void rbtn_jietouEnable_checked();
	void ptn_jietouSimilarity_clicked();
	void ptn_jietouArea_clicked();
	void rbtn_xiaozangwuEnable_checked();
	void ptn_xiaozangwuSimilarity_clicked();
	void ptn_xiaozangwuArea_clicked();
	void rbtn_bodyEnable_checked();
	void pbtn_bodySimilarity_clicked();
	void pbtn_bodyArea_clicked();
	void rbtn_huapoEnable_checked();
	void pbtn_huapoSimilarity_clicked();
	void pbtn_huapoArea_clicked();
	void rbtn_weizhiquexianSingleSimilarityEnable_checked(bool isChecked);
	void pbtn_weizhiquexianSingleSimilarity_clicked();
	void rbtn_weizhiquexianAllAreaEnable_checked(bool isChecked);
	void pbtn_weizhiquexianAllArea_clicked();

signals:
	void scoreFormClosed();

public:
	Ui::DlgProductScoreClass* ui;
};

