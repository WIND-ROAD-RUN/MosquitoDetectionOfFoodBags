#pragma once

#include <QString>

#include "ScoreConfig.hpp"

namespace cdm
{
	struct ScoreConfigConvert
	{
	};
}

namespace osoFileUtilityFunc
{
	static QString defectClassIdToText(int defectClassId)
	{
		switch (defectClassId)
		{
		case 0:
			return "检测到蚊虫缺陷";
		case 1:
			return "检测到毛发缺陷";
		case 2:
			return "检测到蚊虫与毛发缺陷";
		default:
			return "检测到缺陷";
		}
	}
}