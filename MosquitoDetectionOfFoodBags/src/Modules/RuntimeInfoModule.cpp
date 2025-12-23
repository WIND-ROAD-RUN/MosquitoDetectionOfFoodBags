#include "RuntimeInfoModule.hpp"

#include "Modules.hpp"

bool RuntimeInfoModule::build()
{
	detachUtiltyThread = std::make_unique<DetachUtiltyThread>(this);

	auto &configModule=Modules::getInstance().configManagerModule;
	statisticalInfo.productionLength = configModule.MainWindowsConfig.totalProductionLength;
	statisticalInfo.wasteCount = configModule.MainWindowsConfig.totalDefectiveVolume;

	return true;
}

void RuntimeInfoModule::destroy()
{
	detachUtiltyThread.reset();
}

void RuntimeInfoModule::start()
{
	detachUtiltyThread->startThread();
}

void RuntimeInfoModule::stop()
{
	detachUtiltyThread->stopThread();
	detachUtiltyThread->wait();
}
