#include "UIModule.hpp"
#include "DlgProductSet.h"
#include "DlgProductScore.h"
#include "rqw_LabelWarning.h"
#include "ui_DlgProductScore.h"
#include "ui_DlgProductSet.h"
#include "DlgCloseForm.h"

void UIModule::build()
{
	_dlgProductSet = new DlgProductSet();
	_dlgProductScore = new DlgProductScore();
	labelWarning = new rw::rqw::LabelWarning();
	_dlgCloseForm = new DlgCloseForm();

	// 构建分数界面与设置界面的UI联动
	ini_dlgProductScoreGroupList();
	ini_dlgProductSetCheckList();
	connectSetAndScore();
}

void UIModule::destroy()
{
	delete _dlgProductSet;
	delete _dlgProductScore;
	delete labelWarning;
	delete _dlgCloseForm;
}

void UIModule::start()
{

}

void UIModule::stop()
{

}

void UIModule::ini_dlgProductScoreGroupList()
{
	_dlgProductScoreGroupList = {
		_dlgProductScore->ui->widget_zangwu,
		_dlgProductScore->ui->widget_xiaozangwu,
		_dlgProductScore->ui->widget_jietou,
		_dlgProductScore->ui->widget_body,
		_dlgProductScore->ui->widget_huapo,
		_dlgProductScore->ui->widget_weizhiquexian
	};
}

void UIModule::ini_dlgProductSetCheckList()
{
	_dlgProductSetCheckList = {
		_dlgProductSet->ui->ckb_zangwu,
		_dlgProductSet->ui->ckb_xiaozangwu,
		_dlgProductSet->ui->ckb_jietou,
		_dlgProductSet->ui->ckb_body,
		_dlgProductSet->ui->ckb_huapo,
		_dlgProductSet->ui->ckb_weizhiquexian
	};
}

void UIModule::connectSetAndScore()
{
	for (int i = 0; i < _dlgProductSetCheckList.size(); ++i) {
		connect(_dlgProductSetCheckList[i], &QCheckBox::toggled, _dlgProductScoreGroupList[i], &QWidget::setVisible);
		_dlgProductScoreGroupList[i]->setVisible(_dlgProductSetCheckList[i]->isChecked());
	}
}
