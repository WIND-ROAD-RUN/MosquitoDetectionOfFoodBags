#include "ConfigManagerModule.hpp"

#include "Utilty.hpp"
#include "Modules.hpp"

bool ConfigManagerModule::build()
{
    storeContext = std::make_unique<rw::oso::StorageContext>(rw::oso::StorageType::Xml);
	auto& runtimeModule = Modules::getInstance().runtimeInfoModule;

#pragma region readWetPapersCfg
	auto loadMainWindowConfig = storeContext->loadSafe(globalPath.WetPapersConfigPath.toStdString());
	if (loadMainWindowConfig)
	{
		wetPapersConfig = *loadMainWindowConfig;
		runtimeModule.isTakePictures = wetPapersConfig.isSaveImg;
		runtimeModule.statisticalInfo.produceCount = wetPapersConfig.totalProductionVolume;
		runtimeModule.statisticalInfo.wasteCount = wetPapersConfig.totalDefectiveVolume;
		runtimeModule.statisticalInfo.productionYield = wetPapersConfig.productionYield;
	}
#pragma endregion

#pragma region readscoreCfg
	loadMainWindowConfig = storeContext->loadSafe(globalPath.DlgProductScoreConfigPath.toStdString());
	if (loadMainWindowConfig)
	{
		scoreConfig = *loadMainWindowConfig;
	}
#pragma endregion

#pragma region readsetCfg
	loadMainWindowConfig = storeContext->loadSafe(globalPath.DlgProductSetConfigPath.toStdString());
	if (loadMainWindowConfig)
	{
		setConfig = *loadMainWindowConfig;
	}
#pragma endregion

	return true;
}

void ConfigManagerModule::destroy()
{
	storeContext->saveSafe(wetPapersConfig, globalPath.WetPapersConfigPath.toStdString());
	storeContext->saveSafe(scoreConfig, globalPath.DlgProductScoreConfigPath.toStdString());
	storeContext->saveSafe(setConfig, globalPath.DlgProductSetConfigPath.toStdString());
	storeContext.reset();
}

void ConfigManagerModule::start()
{
	wetPapersConfig.isDebug = false;
	wetPapersConfig.isDefect = true;
}

void ConfigManagerModule::stop()
{

}
