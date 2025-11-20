#include "RuntimeInfoModule.hpp"

#include "Modules.hpp"

bool RuntimeInfoModule::build()
{
	detachUtiltyThread = std::make_unique<DetachUtiltyThread>(this);

	auto &configModule=Modules::getInstance().configManagerModule;
	statisticalInfo.produceCount = configModule.wetPapersConfig.totalProductionVolume;
	statisticalInfo.wasteCount = configModule.wetPapersConfig.totalDefectiveVolume;
	statisticalInfo.productionYield = configModule.wetPapersConfig.productionYield;

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
