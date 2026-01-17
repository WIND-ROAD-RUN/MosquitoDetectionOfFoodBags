#pragma once


#include"IModule.hpp"
#include"lgm_ILogger.hpp"
#include"lgms_SpdlogModuleLogger.hpp"

class LogModule
	: public IModule<> {
public:
	bool build() override;
	void destroy() override;
	void start() override;
	void stop() override;
public:
	std::shared_ptr<rw::lgm::SpdlogModuleLogger> FoodPackageLogger{ nullptr };
public:
#pragma region FoodPackageLogger
	void trace(const std::string& message);
	void debug(const std::string& message);
	void info(const std::string& message);
	void warn(const std::string& message);
	void error(const std::string& message);
	void critical(const std::string& message);

	template<typename... Args>
	void trace(const std::string& fmt, Args&&... args) {
		FoodPackageLogger->trace(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void debug(const std::string& fmt, Args&&... args) {
		FoodPackageLogger->debug(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void info(const std::string& fmt, Args&&... args) {
		FoodPackageLogger->info(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void warn(const std::string& fmt, Args&&... args) {
		FoodPackageLogger->warn(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void error(const std::string& fmt, Args&&... args) {
		FoodPackageLogger->error(fmt, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void critical(const std::string& fmt, Args&&... args) {
		FoodPackageLogger->critical(fmt, std::forward<Args>(args)...);
	}
#pragma endregion

public:
	void setLevel(rw::lgm::LogLevel level) ;
	void flush();
};