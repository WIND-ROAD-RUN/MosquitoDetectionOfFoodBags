#include"LogModule.hpp"

#include <QDir>
#include <QString>
#include <QStandardPaths>
#include "lgms_DailyFileSinkManager.hpp"
#include "lgms_RotatingFileSinkManager.hpp"

bool LogModule::build()
{
	rw::lgm::RotatingFileSinkConfig RotatingFileSinkConfig;
	RotatingFileSinkConfig.maxFiles = 1;
	RotatingFileSinkConfig.maxFileSize = 2 * 1024 * 1024 * 1024; // 2GB
	RotatingFileSinkConfig.level = rw::lgm::LogLevel::Debug;

	const std::string mainLoggerName = "FoodPackage";
	
	const QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	const QString logPath = QDir(desktopPath).filePath("Logs.log");
	RotatingFileSinkConfig.filePath = logPath.toStdString();

	RotatingFileSinkConfig.pattern = "[%Y-%m-%d %H:%M:%S.%e] %v";
	rw::lgm::RotatingFileSinkManager::instance().createOrReplaceSink(mainLoggerName, RotatingFileSinkConfig);

	auto& manager = rw::lgm::LoggerManager::getInstance();
	rw::lgm::SpdlogModuleLoggerConfig cfg;
	cfg.async = false;
	cfg.enableConsole = false;

	cfg.level = rw::lgm::LogLevel::Info;
	cfg.consoleSinkLevel = rw::lgm::LogLevel::Info;

	cfg.enableFile = true;
	cfg.fileSinkType = rw::lgm::FileSinkType::Rotating;

	cfg.fileSinkID = mainLoggerName;
	FoodPackageLogger = rw::lgm::SpdlogModuleLogger::create(mainLoggerName, cfg);
	manager.registerLogger(mainLoggerName, FoodPackageLogger);

	return true;
}

void LogModule::destroy()
{
	FoodPackageLogger->flush();
}

void LogModule::start()
{
}

void LogModule::stop()
{
}

void LogModule::trace(const std::string& message)
{
	FoodPackageLogger->trace(message);
}

void LogModule::debug(const std::string& message)
{
	FoodPackageLogger->debug(message);
}

void LogModule::info(const std::string& message)
{
	FoodPackageLogger->info(message);
}

void LogModule::warn(const std::string& message)
{
	FoodPackageLogger->warn(message);
}

void LogModule::error(const std::string& message)
{
	FoodPackageLogger->error(message);
}

void LogModule::critical(const std::string& message)
{
	FoodPackageLogger->critical(message);
}

void LogModule::setLevel(rw::lgm::LogLevel level)
{
	FoodPackageLogger->setLevel(level);
}

void LogModule::flush()
{
	FoodPackageLogger->flush();
}
