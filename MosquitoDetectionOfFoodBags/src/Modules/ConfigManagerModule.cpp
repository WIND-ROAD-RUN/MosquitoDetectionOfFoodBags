#include "ConfigManagerModule.hpp"

#include "Utilty.hpp"
#include "Modules.hpp"

bool ConfigManagerModule::build()
{
    storeContext = std::make_unique<rw::oso::StorageContext>(rw::oso::StorageType::Xml);
	auto& runtimeModule = Modules::getInstance().runtimeInfoModule;

#pragma region readWetPapersCfg
	auto loadMainWindowConfig = storeContext->loadSafe(globalPath.MainWindwsConfigPath.toStdString());
	if (loadMainWindowConfig)
	{
		MainWindowsConfig = *loadMainWindowConfig;
		runtimeModule.isTakePictures = MainWindowsConfig.isSaveImg;
		runtimeModule.statisticalInfo.produceCount = MainWindowsConfig.totalProductionVolume;
		runtimeModule.statisticalInfo.wasteCount = MainWindowsConfig.totalDefectiveVolume;
		runtimeModule.statisticalInfo.productionYield = MainWindowsConfig.productionYield;
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
	storeContext->saveSafe(MainWindowsConfig, globalPath.MainWindwsConfigPath.toStdString());
	storeContext->saveSafe(scoreConfig, globalPath.DlgProductScoreConfigPath.toStdString());
	storeContext->saveSafe(setConfig, globalPath.DlgProductSetConfigPath.toStdString());
	storeContext.reset();
}

void ConfigManagerModule::start()
{
	MainWindowsConfig.isDebug = false;
	MainWindowsConfig.isDefect = true;
}

void ConfigManagerModule::stop()
{

}
