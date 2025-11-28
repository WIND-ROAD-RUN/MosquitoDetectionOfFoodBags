#include "ImgProModule.hpp"

#include "Modules.hpp"
#include "osoFIleUtiltyFunc.hpp"
#include "Utilty.hpp"

bool ImgProModule::build()
{
	buildImgProContext();
	buildImageProcessingModule(imgProSignalWorkThreadNum);
	for (int i = 0; i < 4; i++)
	{
		imgProIsUpdate[i] = true;
	}
	return true;
}

void ImgProModule::destroy()
{
	destroyImageProcessingModule();
}

void ImgProModule::start()
{

}

void ImgProModule::stop()
{

}

void ImgProModule::buildImgProContext()
{
	buildImgProContextMain();
}

void ImgProModule::buildImgProContextMain()
{
	auto& runningState = Modules::getInstance().runtimeInfoModule.runningState;
	auto& mainWindowConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;

}


void ImgProModule::resetImgProIsUpdate(bool state)
{
	for (auto& item : imgProIsUpdate)
	{
		item.store(state);
	}
}

void ImgProModule::buildImageProcessingModule(size_t num)
{
	imageProcessingModule1 = std::make_unique<ImageProcessingModule>(num, this);
	imageProcessingModule1->modelEnginePath = globalPath.modelPath;
	imageProcessingModule1->index = 1;
	imageProcessingModule1->BuildModule();
}

void ImgProModule::destroyImageProcessingModule()
{
	imageProcessingModule1.reset();
}

void ImgProModule::onUpdateImgProContext()
{
	buildImgProContext();
	resetImgProIsUpdate(true);
}
