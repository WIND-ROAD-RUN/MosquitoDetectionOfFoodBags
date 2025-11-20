#pragma once
#include <QString>

#include "imgPro_ImageProcessUtilty.hpp"

struct Utility
{
	static QString cameraIp1;
	static QString cameraIp2;
	static QString zmotionIp;
};

struct ClassId
{
	static constexpr int minNum = 0;
	static constexpr int zangWu = 0;
	static constexpr int xiaoZangWu = 1;
	static constexpr int jieTou = 2;
	static constexpr int body = 3;
	static constexpr int huaPo = 4;

	static constexpr int customClassIdMin = 5;

	static constexpr int weiZhiQueXian = 5;
	static constexpr int weiZhiQueXianTotal = 6;

	static constexpr int maxNum = 6;
	static constexpr int indexCount = 7;
	static std::unordered_map<rw::imgPro::ClassId, rw::imgPro::ClassIdName> classIdNameMap;
	static std::vector<rw::imgPro::ClassId> classids;
};

struct ControlLines
{
public:	// 输入
	
public: // 输出
	static size_t tifeixinhaoOut;
	static size_t xiangjichufa1Out;
	static size_t xiangjichufa2Out;
};

inline struct GlobalPath
{
public:
	QString projectHome = R"(D:\zfkjData\WetPapers\)";
public:
	QString configRootPath = projectHome + R"(config\)";
	QString modelRootPath = projectHome + R"(model\)";
	QString WetPapersConfigPath = configRootPath + R"(WetPapersConfig.xml)";
	QString DlgProductScoreConfigPath = configRootPath + R"(DlgProductScoreConfig.xml)";
	QString DlgProductSetConfigPath = configRootPath + R"(DlgProductSetConfig.xml)";
	QString modelPath = modelRootPath + R"(WetPapers.engine)";
public:
	QString imageSaveRootPath = projectHome + R"(SavedImages\)";

}globalPath;
