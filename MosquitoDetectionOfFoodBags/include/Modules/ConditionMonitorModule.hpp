#pragma once

#include"IModule.hpp"
#include<QObject>

#include "WarnUtilty.hpp"

#include"conMon_ConditionMonitor.hpp"

namespace rw::rqw
{
	class LabelWarning;
}

class DlgWarn;

class ConditionMonitorModule
	:public QObject, public IModule<>
{
	Q_OBJECT
public:
	std::atomic_int warningNum{ 0 };
	void appendWarningNum();
	void reduceWarningNum();
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;
public:
	std::unique_ptr<rw::conMon::ConditionMonitor> conditionMonitor = nullptr;
public:
	QVector<DlgWarn*> dlgWarnVec;
	rw::rqw::LabelWarning* labelWarning = nullptr;
public:
	void addWarning(const rw::rqw::WarningInfo& message, int redDuration);
	void addWarning(const rw::rqw::WarningInfo& message, bool updateTimestampIfSame = true, int redDuration = 5000, int time = 10000);
public slots:
	void onDlgWarningAccept();
};
