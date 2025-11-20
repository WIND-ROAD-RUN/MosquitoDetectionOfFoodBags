#include "osoFIleUtiltyFunc.hpp"

#include "Utilty.hpp"

rw::imgPro::EliminationInfoFunc::ClassIdWithConfigMap cdm::ScoreConfigConvert::toClassIdWithEliConfigMap(
	const cdm::ScoreConfig& config, double areaFactor, double scoreFactor)
{
	rw::imgPro::EliminationInfoFunc::ClassIdWithConfigMap result{};

	rw::imgPro::EliminationInfoGetConfig configBase;
	configBase.isUsingArea = true;
	configBase.isUsingScore = true;
	configBase.areaFactor = areaFactor;
	configBase.scoreFactor = scoreFactor;
	configBase.areaIsUsingComplementarySet = false;
	configBase.scoreIsUsingComplementarySet = false;

	for (int i = ClassId::minNum; i <= ClassId::maxNum; i++)
	{
		result[i] = configBase;
	}

	// body不参与剔废
	result[ClassId::body].isUsingArea = false;
	result[ClassId::body].isUsingScore = false;

	result[ClassId::zangWu].areaRange = { 0,config.zangWuArea };
	result[ClassId::zangWu].scoreRange = { 0,config.zangWuScore };

	result[ClassId::xiaoZangWu].areaRange = { 0,config.xiaoZangWuArea };
	result[ClassId::xiaoZangWu].scoreRange = { 0,config.xiaoZangWuScore };

	result[ClassId::jieTou].areaRange = { 0,config.jieTouArea };
	result[ClassId::jieTou].scoreRange = { 0,config.jieTouScore };

	result[ClassId::huaPo].areaRange = { 0,config.huaPoArea };
	result[ClassId::huaPo].scoreRange = { 0,config.huaPoScore };

	result[ClassId::weiZhiQueXian].isUsingScore = false;
	result[ClassId::weiZhiQueXian].areaRange = { 0,config.weiZhiQueXianSingleArea };

	result[ClassId::weiZhiQueXianTotal].isUsingScore = false;
	result[ClassId::weiZhiQueXianTotal].areaRange = { 0,config.weiZhiQueXianAllArea };

	return result;
}

rw::imgPro::DefectResultInfoFunc::ClassIdWithConfigMap cdm::ScoreConfigConvert::toClassIdWithDefConfigMap(
	const cdm::ScoreConfig& config)
{
	rw::imgPro::DefectResultInfoFunc::ClassIdWithConfigMap result;
	rw::imgPro::DefectResultInfoFunc::Config configBase;
	configBase.isEnable = false;
	for (int i = ClassId::minNum; i <= ClassId::maxNum; i++)
	{
		result[i] = configBase;
	}

	// body不参与剔废
	result[ClassId::body].isEnable = false;

	result[ClassId::zangWu].isEnable = config.zangWu;
	result[ClassId::xiaoZangWu].isEnable = config.xiaoZangWu;
	result[ClassId::jieTou].isEnable = config.jieTou;
	result[ClassId::huaPo].isEnable = config.huaPo;
	result[ClassId::weiZhiQueXian].isEnable = config.weiZhiQueXianSingle;
	result[ClassId::weiZhiQueXianTotal].isEnable = config.weiZhiQueXianAll;

	return result;
}
