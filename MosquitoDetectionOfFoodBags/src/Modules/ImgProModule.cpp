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

#pragma region  build base
	imageProcessContext_Main.imageProcessPrepare = [this, &runningState, &mainWindowConfig](rw::imgPro::ImageProcessContext& context)
		{
			
		};
#pragma endregion

#pragma region build index get
	imageProcessContext_Main.indexGetContext.removeIndicesIfByInfo = [this](const rw::DetectionRectangleInfo& info
		, rw::imgPro::ImageProcessContext& context)
		{
			return false;
		};

	imageProcessContext_Main.indexGetContext.removeIndicesPost =
		[this](rw::imgPro::ProcessResultIndexMap& indexMap,
			const rw::imgPro::ProcessResult& processResult,
			rw::imgPro::ImageProcessContext& context) {

		};
#pragma endregion

#pragma region build elimination config
	imageProcessContext_Main.eliminationInfoGetContext.getEliminationItemFuncSpecialOperator = [this](rw::imgPro::EliminationItem& item,
		const rw::DetectionRectangleInfo& info,
		const rw::imgPro::EliminationInfoGetConfig& cfg) {
			
	};

	imageProcessContext_Main.eliminationInfoGetContext.getEliminationItemPostOperator = [](
		const rw::imgPro::ProcessResult& processResult,
		const rw::imgPro::ProcessResultIndexMap& processResultIndexMap,
		const rw::imgPro::ClassIdWithEliminationInfoConfigMap& classIdWithEliminationInfoConfigMap,
		rw::imgPro::ImageProcessContext& context,
		rw::imgPro::EliminationInfo& eliminationInfo
		)
		{
			
		};
#pragma endregion

#pragma region build defect config
	imageProcessContext_Main.defectResultGetContext.getDefectResultExtraPostOperate = [this](
		const rw::imgPro::ProcessResult& processResult,
		const rw::imgPro::ClassIdWithEliminationInfoConfigMap& classIdWithEliminationInfoConfigMap,
		const rw::imgPro::EliminationInfo& eliminationInfo,
		const rw::imgPro::ClassIdWithDefectResultInfoFuncConfigMap& classIdWithDefectResultInfoFuncConfigMap,
		rw::imgPro::DefectResultInfo& defectResultInfo,
		rw::imgPro::ImageProcessContext& context)
		{

		};
#pragma endregion

#pragma region build defect draw
	imageProcessContext_Main.defectDrawCfg.classIdNameMap = ClassId::classIdNameMap;

	rw::imgPro::DefectDrawConfigItem drawItemConfig;

	drawItemConfig.fontSize = 50;
	drawItemConfig.textLocate = rw::imgPro::ConfigDrawRect::TextLocate::LeftTopIn;
	drawItemConfig.isDrawMask = false;

	for (size_t i = ClassId::minNum; i <= ClassId::maxNum; i++)
	{
		imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[i] = drawItemConfig;
	}

	imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[ClassId::body].isDisAreaText = false;
	imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[ClassId::body].isDisScoreText = false;
	imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[ClassId::body].isDisName = false;
	imageProcessContext_Main.defectDrawCfg.classIdWithConfigMap[ClassId::body].isDrawMask = false;

	imageProcessContext_Main.defectDrawFuncContext.postOperateFunc = [](
		QImage& img,
		rw::imgPro::ImageProcessContext& context) {

		};
#pragma endregion

#pragma region build running time text
	imageProcessContext_Main.runTextCfg.isDisProcessImgTime = false;
	imageProcessContext_Main.runTextCfg.isDrawExtraText = false;
#pragma endregion
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
