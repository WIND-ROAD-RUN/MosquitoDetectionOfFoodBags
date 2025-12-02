#include "UIModule.hpp"
#include "DlgProductSet.h"
#include "DlgProductScore.h"
#include "rqw_LabelWarning.h"
#include "ui_DlgProductScore.h"
#include "ui_DlgProductSet.h"
#include "DlgCloseForm.h"
#include "DlgProductLimit.h"

void UIModule::build()
{
	_dlgProductSet = new DlgProductSet();
	_dlgProductScore = new DlgProductScore();
	labelWarning = new rw::rqw::LabelWarning();
	_dlgCloseForm = new DlgCloseForm();
	_dlgProductLimit = new DlgProductLimit();

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
	delete _dlgProductLimit;
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
		
	};
}

void UIModule::ini_dlgProductSetCheckList()
{
	_dlgProductSetCheckList = {
		
	};
}

void UIModule::connectSetAndScore()
{
	/*for (int i = 0; i < _dlgProductSetCheckList.size(); ++i) {
		connect(_dlgProductSetCheckList[i], &QCheckBox::toggled, _dlgProductScoreGroupList[i], &QWidget::setVisible);
		_dlgProductScoreGroupList[i]->setVisible(_dlgProductSetCheckList[i]->isChecked());
	}*/
}
