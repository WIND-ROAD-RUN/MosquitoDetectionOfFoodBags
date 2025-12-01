#pragma once

#include <QCheckBox>
#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class DlgProductSetClass; };
QT_END_NAMESPACE

class DlgProductSet : public QDialog
{
	Q_OBJECT

public:
	DlgProductSet(QWidget* parent = nullptr);
	~DlgProductSet();

public:
	void build_ui();
	void read_config();
	void build_connect();

private:
	struct DOCheckItem {
		size_t* line;
		QCheckBox* box;
	};
	std::vector<DOCheckItem> DOCheckItems;
	void initDOCheckItems();
	std::vector<DOCheckItem> DICheckItems;
	void initDICheckItems();

	std::vector<std::vector<int>> DOFindAllDuplicateIndices();
	void setDOErrorInfo(const std::vector<std::vector<int>>& index);
	void setDOErrorInfo(int index);
	std::vector<std::vector<int>> DIFindAllDuplicateIndices();
	void setDIErrorInfo(const std::vector<std::vector<int>>& index);
	void setDIErrorInfo(int index);

	void updateControlLines();
signals:
	void pixToWorldChanged();
	void tifeijuliChanged();

private:
	// IO监控页面的调试模式
	bool isDebugIO{ false };

private slots:
	void pbtn_close_clicked();

	void pbtn_tifeichixushijian_clicked();
	void pbtn_tifeijuli_clicked();
	void pbtn_shangxianwei_clicked();
	void pbtn_xiaxianwei_clicked();
	void pbtn_zuoxianwei_clicked();
	void pbtn_youxianwei_clicked();
	void pbtn_baoguang_clicked();
	void pbtn_zengyi_clicked();
	void pbtn_xiangsudangliang_clicked();

	void cbox_yundongkongzhiqichonglian_checked();

	// 基本参数
	void btn_xiangjichufachangdu_clicked();
	void btn_meizhuanmaichongshu_clicked();
	void btn_shedingzhouchang_clicked();

	// 监控IO
	void cbox_debugMode_checked(bool ischecked);

	void cbox_DOtifeixinhao_checked(bool ischecked);
	void cbox_DOxiangjichufa1_checked(bool ischecked);
	void cbox_DOxiangjichufa2_checked(bool ischecked);

	// 设置IO
	void btn_setDOtifeixinhao_clicked();
	void btn_setDOxiangjichufa1_clicked();
	void btn_setDOxiangjichufa2_clicked();

	void tabWidget_indexChanged(int index);

public slots:
	// 监控IO函数
	void monitorInPutSignal(size_t index, bool state);
	void monitorOutPutSignal(size_t index, bool state);

public:
	Ui::DlgProductSetClass* ui;
};

