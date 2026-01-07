#include "ConditionMonitorModule.hpp"
#include "DlgWarn.h"
#include "Modules.hpp"

void ConditionMonitorModule::appendWarningNum()
{
	++warningNum;
}

void ConditionMonitorModule::reduceWarningNum()
{
	--warningNum;
	if (warningNum < 0)
	{
		warningNum = 0;
	}
}

bool ConditionMonitorModule::build()
{
	for (int i = 0; i < WarningId::warnNum; i++)
	{
		auto newDlgWarn = new DlgWarn;
		dlgWarnVec.append(newDlgWarn);
		QObject::connect(newDlgWarn, &DlgWarn::isProcess,
			this, &ConditionMonitorModule::onDlgWarningAccept);
	}

	labelWarning = new rw::rqw::LabelWarning();
	labelWarning->setMaxHistorySize(500);
	// 设置右下角警告图标灰色持续时间为无限大
	labelWarning->setGrayDuration(std::numeric_limits<int>::max());

	auto setConfig = Modules::getInstance().configManagerModule.setConfig;

	rw::conMon::ConditionCfg cfg;
	cfg.async = true;
	cfg.intervalMs = 200;
	conditionMonitor = std::make_unique<rw::conMon::ConditionMonitor>(cfg);

#pragma region conditionCamera
	rw::conMon::ConditionItem conditionCamera;
	conditionCamera.info.id = WarningId::cameraConnectionLost;
	conditionCamera.info.name = QString(QTime::currentTime().toString() + "相机断开连接报警");
	conditionCamera.set.enabled = setConfig.xiangjiduanlianbaojing;
	conditionCamera.call.condition = [](const rw::conMon::ConditionInfo& info) -> bool
		{
			auto& cameraModule = Modules::getInstance().cameraModule;
			if (cameraModule.camera1)
			{
				return !cameraModule.camera1->getConnectState();
			}
			return true;
		};
	conditionCamera.call.onTrigger = [this](const rw::conMon::ConditionInfo& info)
		{
			rw::rqw::WarningInfo WarningInfo;
			WarningInfo.message = info.name;
			WarningInfo.type = rw::rqw::WarningType::Warning;
			WarningInfo.warningId = WarningId::cameraConnectionLost;

			QMetaObject::invokeMethod(this,
				[this, WarningInfo]() {
					addWarning(WarningInfo);
					auto& dlgWarn = dlgWarnVec[WarningId::cameraConnectionLost];
					dlgWarn->setTime(QDateTime::currentDateTime().toString("yyyy年MM月dd日"));
					dlgWarn->setTitle("警告");
					dlgWarn->setText(WarningInfo.message);
					dlgWarn->show();
				});

			appendWarningNum();
		};
	conditionCamera.call.onClear = [this](const rw::conMon::ConditionInfo& info)
		{
			reduceWarningNum();
			auto& dlgWarn = dlgWarnVec[WarningId::cameraConnectionLost];

			QMetaObject::invokeMethod(this,
				[this, dlgWarn]() {
					dlgWarn->close();
				});
		};

	conditionMonitor->registerCondition(conditionCamera);
#pragma endregion

#pragma region conditionZmotion
	rw::conMon::ConditionItem conditionZmotion;
	conditionZmotion.info.id = WarningId::zmotionConnectionLost;
	conditionZmotion.info.name = QString(QTime::currentTime().toString() + "运动控制器断开连接报警");
	conditionZmotion.set.enabled = setConfig.yundongkongzhiqiduanlianbaojing;
	conditionZmotion.call.condition = [](const rw::conMon::ConditionInfo& info) -> bool
		{
			auto& motionControllerModule = Modules::getInstance().motionControllerModule;
			if (motionControllerModule.zmotion)
			{
				return !motionControllerModule.zmotion->getConnectState();
			}
			return true;
		};
	conditionZmotion.call.onTrigger = [this](const rw::conMon::ConditionInfo& info)
		{
			rw::rqw::WarningInfo WarningInfo;
			WarningInfo.message = info.name;
			WarningInfo.type = rw::rqw::WarningType::Warning;
			WarningInfo.warningId = WarningId::zmotionConnectionLost;
			QMetaObject::invokeMethod(this,
				[this, WarningInfo]() {
					addWarning(WarningInfo);
					auto& dlgWarn = dlgWarnVec[WarningId::zmotionConnectionLost];
					dlgWarn->setTime(QDateTime::currentDateTime().toString("yyyy年MM月dd日"));
					dlgWarn->setTitle("警告");
					dlgWarn->setText(WarningInfo.message);
					dlgWarn->show();
				});
			appendWarningNum();
		};
	conditionZmotion.call.onClear = [this](const rw::conMon::ConditionInfo& info)
		{
			reduceWarningNum();
			auto& dlgWarn = dlgWarnVec[WarningId::zmotionConnectionLost];
			QMetaObject::invokeMethod(this,
				[this, dlgWarn]() {
					dlgWarn->close();
				});
		};
	conditionMonitor->registerCondition(conditionZmotion);
#pragma endregion

#pragma region conditionTiebiaotiewan
	rw::conMon::ConditionItem conditionTiebiaotiewan;
	conditionTiebiaotiewan.info.id = WarningId::jiaodaiTieWan;
	conditionTiebiaotiewan.info.name = "贴标机胶带用完报警";
	conditionTiebiaotiewan.set.enabled = setConfig.tiebiaotiewanbaojing;
	conditionTiebiaotiewan.call.condition = [](const rw::conMon::ConditionInfo& info) -> bool
		{
			auto& motionControllerModule = Modules::getInstance().motionControllerModule;
			if (motionControllerModule.zmotion)
			{
				auto isUsedOut = motionControllerModule.zmotion->getIOIn(ControlLines::jiaodaitiewanIn);
				// 胶带贴完了
				if (!isUsedOut)
				{
					return false;
				}
			}
			return true;
		};
	conditionTiebiaotiewan.call.onTrigger = [this](const rw::conMon::ConditionInfo& info)
		{
			rw::rqw::WarningInfo WarningInfo;
			WarningInfo.message = info.name;
			WarningInfo.type = rw::rqw::WarningType::Warning;
			WarningInfo.warningId = WarningId::jiaodaiTieWan;
			QMetaObject::invokeMethod(this,
				[this, WarningInfo]() {
					addWarning(WarningInfo);
					auto& dlgWarn = dlgWarnVec[WarningId::jiaodaiTieWan];
					dlgWarn->setTime(QDateTime::currentDateTime().toString("yyyy年MM月dd日"));
					dlgWarn->setTitle("警告");
					dlgWarn->setText(WarningInfo.message);
					dlgWarn->show();
				});
			appendWarningNum();
		};
	conditionTiebiaotiewan.call.onClear = [this](const rw::conMon::ConditionInfo& info)
		{
			reduceWarningNum();
			auto& dlgWarn = dlgWarnVec[WarningId::jiaodaiTieWan];
			QMetaObject::invokeMethod(this,
				[this, dlgWarn]() {
					dlgWarn->close();
				});
		};
	conditionMonitor->registerCondition(conditionTiebiaotiewan);
#pragma endregion

	return true;
}

void ConditionMonitorModule::destroy()
{
	conditionMonitor->destroyed();
	conditionMonitor.reset();
	delete labelWarning;
	for (auto& item : dlgWarnVec)
	{
		delete item;
	}
}

void ConditionMonitorModule::start()
{
	conditionMonitor->start();
}

void ConditionMonitorModule::stop()
{
	conditionMonitor->stop();
}

void ConditionMonitorModule::addWarning(const rw::rqw::WarningInfo& message, int redDuration)
{
	labelWarning->addWarning(message, redDuration);
}

void ConditionMonitorModule::addWarning(const rw::rqw::WarningInfo& message, bool updateTimestampIfSame,
	int redDuration, int time)
{
	labelWarning->addWarning(message, updateTimestampIfSame, redDuration, time);
}

void ConditionMonitorModule::onDlgWarningAccept()
{
	--warningNum;
	if (warningNum < 0)
	{
		warningNum = 0;
	}
}
