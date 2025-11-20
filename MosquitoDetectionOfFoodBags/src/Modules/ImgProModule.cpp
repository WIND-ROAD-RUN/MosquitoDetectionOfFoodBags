#include "ImgProModule.hpp"

#include "Modules.hpp"
#include "osoFIleUtiltyFunc.hpp"
#include "Utilty.hpp"

void findHolesInMask(
	std::vector<rw::DetectionRectangleInfo>& hollowRects,
	const std::pair<int, int>& roiOffset,
	int ignoreBorderX,
	int ignoreBorderY,
	const rw::DetectionRectangleInfo& bodyInfo
)
{
	auto & mask = bodyInfo.mask_roi;

	hollowRects.clear();

	// 支持CV_8UC1和CV_32FC1输入
	cv::Mat mask8u;
	if (mask.empty())
		return;

	if (mask.type() == CV_8UC1) {
		mask8u = mask;
	}
	else if (mask.type() == CV_32FC1) {
		// 假设float掩码为0~1，转换到0~255
		mask.convertTo(mask8u, CV_8UC1, 255.0);
	}
	else {
		// 其他类型不支持
		return;
	}

	// 1. 反转mask，空心区域为255，其余为0
	cv::Mat invMask;
	cv::bitwise_not(mask8u, invMask);

	// 2. 标记与边界相连的区域（非空心），将其设为0
	cv::Mat borderMask = invMask.clone();
	cv::Mat visited = cv::Mat::zeros(invMask.size(), CV_8U);
	std::queue<cv::Point> q;

	// 上下左右边界
	for (int x = 0; x < invMask.cols; ++x) {
		if (borderMask.at<uchar>(0, x) == 255) {
			q.push(cv::Point(x, 0));
			visited.at<uchar>(0, x) = 1;
		}
		if (borderMask.at<uchar>(invMask.rows - 1, x) == 255) {
			q.push(cv::Point(x, invMask.rows - 1));
			visited.at<uchar>(invMask.rows - 1, x) = 1;
		}
	}
	for (int y = 0; y < invMask.rows; ++y) {
		if (borderMask.at<uchar>(y, 0) == 255) {
			q.push(cv::Point(0, y));
			visited.at<uchar>(y, 0) = 1;
		}
		if (borderMask.at<uchar>(y, invMask.cols - 1) == 255) {
			q.push(cv::Point(invMask.cols - 1, y));
			visited.at<uchar>(y, invMask.cols - 1) = 1;
		}
	}

	// BFS，将与边界连通的255全部设为0
	const int dx[4] = { 1, -1, 0, 0 };
	const int dy[4] = { 0, 0, 1, -1 };
	while (!q.empty()) {
		auto pt = q.front(); q.pop();
		borderMask.at<uchar>(pt) = 0;
		for (int d = 0; d < 4; ++d) {
			int nx = pt.x + dx[d], ny = pt.y + dy[d];
			if (nx >= 0 && nx < invMask.cols && ny >= 0 && ny < invMask.rows) {
				if (borderMask.at<uchar>(ny, nx) == 255 && !visited.at<uchar>(ny, nx)) {
					q.push(cv::Point(nx, ny));
					visited.at<uchar>(ny, nx) = 1;
				}
			}
		}
	}

	// 3. 剩下的255区域即为空心区域，使用findContours统计面积和外接矩形
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(borderMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	for (const auto& contour : contours) {
		int area = static_cast<int>(cv::contourArea(contour));
		if (area > 0) {

			cv::Rect rect = cv::boundingRect(contour);

			// 边界忽略处理
			if (ignoreBorderX > 0 || ignoreBorderY > 0)
			{
				auto center_x = rect.x + rect.width / 2 + roiOffset.first;
				auto center_y= rect.y + rect.height / 2 + roiOffset.second;
				auto left = bodyInfo.leftBottom.first + ignoreBorderX;
				auto right = bodyInfo.rightBottom.first - ignoreBorderX;
				auto top = bodyInfo.leftTop.second + ignoreBorderY;
				auto bottom = bodyInfo.leftBottom.second - ignoreBorderY;
				// 判断是否在边缘忽略范围内
				if (center_x<left ||
					center_x > right ||
					center_y < top ||
					center_y > bottom)
				{
					continue;
				}
			}


			rw::DetectionRectangleInfo info;
			// 坐标加上roiOffset，变为全图坐标
			info.leftTop = { rect.x + roiOffset.first, rect.y + roiOffset.second };
			info.rightTop = { rect.x + rect.width - 1 + roiOffset.first, rect.y + roiOffset.second };
			info.leftBottom = { rect.x + roiOffset.first, rect.y + rect.height - 1 + roiOffset.second };
			info.rightBottom = { rect.x + rect.width - 1 + roiOffset.first, rect.y + rect.height - 1 + roiOffset.second };
			info.center_x = rect.x + rect.width / 2 + roiOffset.first;
			info.center_y = rect.y + rect.height / 2 + roiOffset.second;
			info.width = rect.width;
			info.height = rect.height;
			info.area = area;
			info.classId = 0;
			info.score = -1;
			info.mask_roi = cv::Mat(borderMask, rect).clone();
			info.roi = cv::Rect(rect.x + roiOffset.first, rect.y + roiOffset.second, rect.width, rect.height);
			info.segMaskValid = true;

			hollowRects.push_back(std::move(info));
		}
	}
}

std::vector<rw::DetectionRectangleInfo> filterProWithShieldWithoutBody(const std::vector<rw::DetectionRectangleInfo>& hollowRects,
	const std::vector<rw::DetectionRectangleInfo>& shields)
{
	std::vector<rw::DetectionRectangleInfo> result;
	for (const auto& item : hollowRects)
	{
		bool isInShield = false;
		for (const auto& shield : shields)
		{
			if (shield.classId==ClassId::body)
			{
				continue;
			}
			isInShield = rw::DetectionRectangleInfo::findPointIsInOtherDet({ item.center_x,item.center_y }, shield, 10);
			if (isInShield)
			{
				break;
			}
		}
		if (!isInShield)
		{
			result.push_back(item);
		}
	}

	return result;
}

int countProsArea(const std::vector<rw::DetectionRectangleInfo>& pros)
{
	int totalArea = 0;
	for (const auto& item : pros)
	{
		totalArea += static_cast<int>(item.area);
	}
	return totalArea;
}

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
	auto& wetPapersConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;

#pragma region  build base
	imageProcessContext_Main.imageProcessPrepare = [this, &runningState, &wetPapersConfig](rw::imgPro::ImageProcessContext& context)
		{
			auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
			auto& scoreConfig = Modules::getInstance().configManagerModule.scoreConfig;

			context.customFields["hasBody"] = false;
			context.customFields["LocationTopY"] = static_cast<int>(0);
			context.customFields["LocationBottomY"] = static_cast<int>(0);
			context.customFields["DefectBox"] = std::vector<int>{};

			if (context.customFields.find("ImgProcessIndex") == context.customFields.end()) {
				return;
			}
			auto ImgProcessIndex = std::any_cast<int>(context.customFields["ImgProcessIndex"]);

			if (context.customFields.find("stationIdx") == context.customFields.end()) {
				return;
			}
			auto stationIdx = std::any_cast<int>(context.customFields["stationIdx"]);

			double currentPixToWorld{ 0 };
			double tifeijuli{ 0 };

			//update pixToWorld && tifeijuli
			{
				if (1 == ImgProcessIndex)
				{
					currentPixToWorld = setConfig.xiangSuDangLiang1;
					tifeijuli = setConfig.tifeijuli1;
				}
				else if (2 == ImgProcessIndex)
				{
					currentPixToWorld = setConfig.xiangSuDangLiang2;
					tifeijuli = setConfig.tifeijuli2;
				}
				context.customFields["CurrentPixToWorld"] = static_cast<double>(currentPixToWorld);
				context.customFields["TiFeiJuLi"] = static_cast<double>(tifeijuli);
			}

			//update Config
			{
				if (imgProIsUpdate[stationIdx])
				{
					context.eliminationCfg = cdm::ScoreConfigConvert::toClassIdWithEliConfigMap(scoreConfig, currentPixToWorld * currentPixToWorld, 100);
					context.defectCfg = cdm::ScoreConfigConvert::toClassIdWithDefConfigMap(scoreConfig);
					imgProIsUpdate[stationIdx] = false;
				}
			}

			// update limite
			{
				int limitLeft{ 0 };
				int limitRight{ 0 };
				int limitTop{ 0 };
				int limitBottom{ 0 };

				if (1 == ImgProcessIndex)
				{
					limitLeft = static_cast<int>(setConfig.zuoXianWei1);
					limitRight = static_cast<int>(setConfig.youXianWei1);
					limitTop = static_cast<int>(setConfig.shangXianWei1);
					limitBottom = static_cast<int>(setConfig.xiaXianWei1);
					currentPixToWorld = setConfig.xiangSuDangLiang1;
				}
				else if (2 == ImgProcessIndex)
				{
					limitLeft = static_cast<int>(setConfig.zuoXianWei2);
					limitRight = static_cast<int>(setConfig.youXianWei2);
					limitTop = static_cast<int>(setConfig.shangXianWei2);
					limitBottom = static_cast<int>(setConfig.xiaXianWei2);
					currentPixToWorld = setConfig.xiangSuDangLiang2;
				}

				context.customFields["LimitLeft"] = static_cast<int>(limitLeft);
				context.customFields["LimitRight"] = static_cast<int>(limitRight);
				context.customFields["LimitTop"] = static_cast<int>(limitTop);
				context.customFields["LimitBottom"] = static_cast<int>(limitBottom);
			}

			//update defect ignore loc
			{
				context.customFields["DefectIgnoreX"] = static_cast<int>(setConfig.defectIgnoreX);
				context.customFields["DefectIgnoreY"] = static_cast<int>(setConfig.defectIgnoreY);
			}

			// update drawConfig
			{
				if (RunningState::Debug == runningState)
				{
					context.defectDrawCfg.textLocate = rw::imgPro::ConfigDrawRect::TextLocate::LeftTopIn;

					if (wetPapersConfig.isshibiekuang)
					{
						context.defectDrawCfg.isDrawDefects = true;
						context.defectDrawCfg.isDrawDisableDefects = true;
						context.defectDrawCfg.isDisAreaText = true;
						context.defectDrawCfg.isDisScoreText = true;
					}
					else
					{
						context.defectDrawCfg.isDrawDefects = false;
						context.defectDrawCfg.isDrawDisableDefects = false;
						context.defectDrawCfg.isDisAreaText = false;
						context.defectDrawCfg.isDisScoreText = false;
					}

					if (wetPapersConfig.iswenzi)
					{
						context.runTextCfg.isDrawExtraText = true;
					}
					else
					{
						context.runTextCfg.isDrawExtraText = false;
					}
				}
				else if (RunningState::OpenRemoveFunc == runningState)
				{
					context.defectDrawCfg.isDrawDefects = true;
					context.defectDrawCfg.isDrawDisableDefects = true;
					context.defectDrawCfg.isDisAreaText = true;
					context.defectDrawCfg.isDisScoreText = true;

					context.runTextCfg.isDrawExtraText = false;
				}
			}
		};
#pragma endregion

#pragma region build index get
	imageProcessContext_Main.indexGetContext.removeIndicesIfByInfo = [this](const rw::DetectionRectangleInfo& info
		, rw::imgPro::ImageProcessContext& context)
		{
			bool isInShieldWires = false;
			int limitLeft{ -1 };
			int limitRight{ -1 };
			int limitTop{ -1 };
			int limitBottom{ -1 };

			if (context.customFields.find("LimitLeft") != context.customFields.end()) {
				limitLeft = std::any_cast<int>(context.customFields["LimitLeft"]);
			}
			if (context.customFields.find("LimitRight") != context.customFields.end()) {
				limitRight = std::any_cast<int>(context.customFields["LimitRight"]);
			}
			if (context.customFields.find("LimitTop") != context.customFields.end()) {
				limitTop = std::any_cast<int>(context.customFields["LimitTop"]);
			}
			if (context.customFields.find("LimitBottom") != context.customFields.end()) {
				limitBottom = std::any_cast<int>(context.customFields["LimitBottom"]);
			}

			if (-1 == limitLeft || -1 == limitRight || -1 == limitTop || -1 == limitBottom)
			{
				return false;
			}
			// 判断缺陷框中心点是否在屏蔽线区域内
			if (info.center_x < limitRight && info.center_x > limitLeft)
			{
				if (info.center_y > limitTop && info.center_y < limitBottom)
				{
					isInShieldWires = true;
				}
			}

			return !isInShieldWires;
		};

	imageProcessContext_Main.indexGetContext.removeIndicesPost =
		[this](rw::imgPro::ProcessResultIndexMap& indexMap,
			const rw::imgPro::ProcessResult& processResult,
			rw::imgPro::ImageProcessContext& context) {

				rw::imgPro::ProcessResultIndexMap newBlankIndexMap{};

				// if has body
				if (true == std::any_cast<bool>(context.customFields["hasBody"]))
				{
					// find max body
					long maxArea = 0;
					rw::imgPro::ProcessResultIndex maxBodyIndex = -1;
					for (const auto& bodyIndex : indexMap[ClassId::body])
					{
						if (processResult[bodyIndex].area > maxArea)
						{
							maxArea = processResult[bodyIndex].area;
							maxBodyIndex = bodyIndex;
						}
					}

					// get defects in maxBody

					const auto& maxBodyProcessResult = processResult[maxBodyIndex];

					for (const auto& defectPairs : indexMap)
					{
						auto& classId = defectPairs.first;
						if (classId != ClassId::body)
						{
							for (const auto& currentDefectIndex : defectPairs.second)
							{
								const auto& currentDefectProcessResult = processResult[currentDefectIndex];
								if (currentDefectProcessResult.center_x > maxBodyProcessResult.leftBottom.first &&
									currentDefectProcessResult.center_x < maxBodyProcessResult.rightTop.first &&
									currentDefectProcessResult.center_y < maxBodyProcessResult.leftBottom.second &&
									currentDefectProcessResult.center_y > maxBodyProcessResult.rightTop.second)
								{
									newBlankIndexMap[classId].insert(currentDefectIndex);
								}
							}
						}
					}
				}
				// if no body
				else
				{
					for (const auto& defectPairs : indexMap)
					{
						auto& classId = defectPairs.first;
						for (const auto& currentDefectIndex : defectPairs.second)
						{
							newBlankIndexMap[classId].insert(currentDefectIndex);
						}
					}
				}

				indexMap.clear();
				for (int i = 0; i < ClassId::maxNum; i++)
				{
					indexMap[i];
				}

				for (const auto& pairs : newBlankIndexMap)
				{
					auto& classId = pairs.first;
					for (const auto& index : pairs.second)
					{
						indexMap[classId].insert(index);
					}
				}
		};
#pragma endregion

#pragma region build elimination config
	imageProcessContext_Main.eliminationInfoGetContext.getEliminationItemFuncSpecialOperator = [this](rw::imgPro::EliminationItem& item,
		const rw::DetectionRectangleInfo& info,
		const rw::imgPro::EliminationInfoGetConfig& cfg) {
			item.customFields["LocationTopY"] = static_cast<int>(info.leftTop.second);
			item.customFields["LocationBottomY"] = static_cast<int>(info.leftBottom.second);
	};

	imageProcessContext_Main.eliminationInfoGetContext.getEliminationItemPostOperator = [](
		const rw::imgPro::ProcessResult& processResult,
		const rw::imgPro::ProcessResultIndexMap& processResultIndexMap,
		const rw::imgPro::ClassIdWithEliminationInfoConfigMap& classIdWithEliminationInfoConfigMap,
		rw::imgPro::ImageProcessContext& context,
		rw::imgPro::EliminationInfo& eliminationInfo
		)
		{
			eliminationInfo.defectItems[ClassId::weiZhiQueXian];
			if (processResultIndexMap.find(ClassId::body)== processResultIndexMap.end())
			{
				return;
			}
			if (processResultIndexMap.at(ClassId::body).empty())
			{
				return;
			}
		//查找空洞
			auto& bodyIndex = *(processResultIndexMap.at(ClassId::body).begin());
			auto& bodyInfo = processResult[bodyIndex];
			std::vector<rw::DetectionRectangleInfo> hollowRects;

			int defectIgnoreX = 0;
			if (context.customFields.find("DefectIgnoreX") != context.customFields.end())
			{
				defectIgnoreX = std::any_cast<int>(context.customFields["DefectIgnoreX"]);
			}

			int defectIgnoreY = 0;
			if (context.customFields.find("DefectIgnoreY") != context.customFields.end())
			{
				defectIgnoreY = std::any_cast<int>(context.customFields["DefectIgnoreY"]);
			}

			double currentPixToWorld = 0;
			if (context.customFields.find("CurrentPixToWorld") != context.customFields.end())
			{
				currentPixToWorld = std::any_cast<double>(context.customFields["CurrentPixToWorld"]);
			}

			defectIgnoreX = static_cast<int>(static_cast<double>(defectIgnoreX) / currentPixToWorld);
			defectIgnoreY = static_cast<int>(static_cast<double>(defectIgnoreY) / currentPixToWorld);
			//find hollow in body
			findHolesInMask(hollowRects, bodyInfo.leftTop, defectIgnoreX, defectIgnoreY, bodyInfo);
			//Filter out hollow that have been identified as defects by AI
			auto findHollowRects= filterProWithShieldWithoutBody(hollowRects,processResult);
			auto totalWeiZhiQueXianArea = countProsArea(findHollowRects);

			auto weizhiquexianScoreCfg = classIdWithEliminationInfoConfigMap.at(ClassId::weiZhiQueXian);

			auto& processResultNoConst = context.processResult();
			for (const auto & item: findHollowRects)
			{
				rw::imgPro::EliminationItem eliminationItem;
				eliminationItem.area = item.area * weizhiquexianScoreCfg.areaFactor;
				eliminationItem.classId = ClassId::weiZhiQueXian;

				processResultNoConst.push_back(item);
				eliminationItem.index = processResultNoConst.size()-1;
				auto isInAreaRange = eliminationItem.area <= weizhiquexianScoreCfg.areaRange.second && eliminationItem.area >= weizhiquexianScoreCfg.areaRange.first;
				if (!isInAreaRange)
				{
					eliminationItem.isBad = true;
				}
				else
				{
					eliminationItem.isBad = false;
				}
				eliminationItem.score = -1;
				eliminationItem.customFields["LocationTopY"] = static_cast<int>(item.leftTop.second);
				eliminationItem.customFields["LocationBottomY"] = static_cast<int>(item.leftBottom.second);

				eliminationInfo.defectItems[ClassId::weiZhiQueXian].push_back(eliminationItem);
			}

			if (!findHollowRects.empty())
			{
				auto wiezhiquexianTotalScoreCfg = classIdWithEliminationInfoConfigMap.at(ClassId::weiZhiQueXianTotal);

				rw::imgPro::EliminationItem eliminationItem;
				eliminationItem.area= totalWeiZhiQueXianArea * weizhiquexianScoreCfg.areaFactor;
				eliminationItem.classId = ClassId::weiZhiQueXianTotal;

				rw::DetectionRectangleInfo info;
				info.area = totalWeiZhiQueXianArea;
				info.center_x = bodyInfo.center_x;
				info.center_y = bodyInfo.center_y;
				info.height = bodyInfo.height;
				info.width = bodyInfo.width;
				info.leftTop = bodyInfo.leftTop;
				info.leftBottom = bodyInfo.leftBottom;
				info.rightTop = bodyInfo.rightTop;
				info.rightBottom = bodyInfo.rightBottom;
				info.classId = ClassId::weiZhiQueXianTotal;
				info.score = -1;
				processResultNoConst.push_back(info);
				eliminationItem.index = processResultNoConst.size() - 1;

				auto isInAreaRange = eliminationItem.area <= wiezhiquexianTotalScoreCfg.areaRange.second && eliminationItem.area >= wiezhiquexianTotalScoreCfg.areaRange.first;
				if (!isInAreaRange)
				{
					eliminationItem.isBad = true;
				}
				else
				{
					eliminationItem.isBad = false;
				}
				eliminationItem.customFields["LocationTopY"] = static_cast<int>(bodyInfo.leftTop.second);
				eliminationItem.customFields["LocationBottomY"] = static_cast<int>(bodyInfo.leftBottom.second);


				eliminationInfo.defectItems[ClassId::weiZhiQueXianTotal].push_back(eliminationItem);
			}
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
			
			auto weiZhiQueXianScoreCfg = classIdWithDefectResultInfoFuncConfigMap.at(ClassId::weiZhiQueXian);
			auto weiZhiQueXianTotalScoreCfg = classIdWithDefectResultInfoFuncConfigMap.at(ClassId::weiZhiQueXianTotal);

		if (weiZhiQueXianScoreCfg.isEnable || weiZhiQueXianTotalScoreCfg.isEnable)
		{
			auto& defects = defectResultInfo.defects;
			auto& disableDefects = defectResultInfo.disableDefects;

			std::unordered_map<rw::imgPro::ClassId, std::vector<rw::imgPro::EliminationItem>> tempDisableDefects;

			for (const auto& pairs : disableDefects)
			{
				rw::imgPro::ClassId classId = pairs.first;


				for (const auto& item : pairs.second)
				{
					if (classId== ClassId::weiZhiQueXian && weiZhiQueXianScoreCfg.isEnable)
					{
						if (item.isBad)
						{
							defects[classId].push_back(item);
						}
						else
						{
							tempDisableDefects[classId].push_back(item);
						}
					}
					else if (classId == ClassId::weiZhiQueXianTotal && weiZhiQueXianTotalScoreCfg.isEnable)
					{
						if (item.isBad)
						{
							defects[classId].push_back(item);
						}
						else
						{
							tempDisableDefects[classId].push_back(item);
						}
					}
					else
					{
						tempDisableDefects[classId].push_back(item);
					}
				}

			}

			defectResultInfo.disableDefects = tempDisableDefects;
			if (!defects.empty())
			{
				defectResultInfo.isBad = true;
			}
			else
			{
				defectResultInfo.isBad = false;
			}
		}
		


			std::vector<int> DefectBox;
			for (const auto& defects : defectResultInfo.defects)
			{
				for (const auto& eliminationItem : defects.second)
				{
					auto findTopY = eliminationItem.customFields.find("LocationTopY");
					auto findBottomY = eliminationItem.customFields.find("LocationBottomY");
					auto pixToWorld = context.customFields.find("CurrentPixToWorld");
					auto tifeijuli = context.customFields.find("TiFeiJuLi");
					if (findTopY != eliminationItem.customFields.end() && 
						findBottomY != eliminationItem.customFields.end() &&
						pixToWorld != context.customFields.end() &&
						tifeijuli != context.customFields.end())
					{
						int topY = std::any_cast<int>(findTopY->second) * std::any_cast<double>(pixToWorld->second) + std::any_cast<double>(tifeijuli->second);
						int bottomY = std::any_cast<int>(findBottomY->second) * std::any_cast<double>(pixToWorld->second) + std::any_cast<double>(tifeijuli->second);
						DefectBox.push_back(topY);
						DefectBox.push_back(bottomY);
					}
				}
			}

			context.customFields["DefectBox"] = DefectBox;
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

			int limitLeft{ 0 };
			int limitRight{ 0 };
			int limitTop{ 0 };
			int limitBottom{ 0 };

			if (context.customFields.find("LimitLeft") != context.customFields.end()) {
				limitLeft = std::any_cast<int>(context.customFields["LimitLeft"]);
			}
			if (context.customFields.find("LimitRight") != context.customFields.end()) {
				limitRight = std::any_cast<int>(context.customFields["LimitRight"]);
			}
			if (context.customFields.find("LimitTop") != context.customFields.end()) {
				limitTop = std::any_cast<int>(context.customFields["LimitTop"]);
			}
			if (context.customFields.find("LimitBottom") != context.customFields.end()) {
				limitBottom = std::any_cast<int>(context.customFields["LimitBottom"]);
			}

			rw::imgPro::ConfigDrawLine configDrawLine;
			configDrawLine.color = rw::imgPro::Color::Red;
			configDrawLine.thickness = 20;

			configDrawLine.position = limitTop;
			rw::imgPro::ImagePainter::drawHorizontalLine(img, configDrawLine);
			configDrawLine.position = limitBottom;
			rw::imgPro::ImagePainter::drawHorizontalLine(img, configDrawLine);
			configDrawLine.position = limitLeft;
			rw::imgPro::ImagePainter::drawVerticalLine(img, configDrawLine);
			configDrawLine.position = limitRight;
			rw::imgPro::ImagePainter::drawVerticalLine(img, configDrawLine);
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
	imageProcessingModule1 = std::make_unique<ImageProcessingModuleWetPapers>(num, this);
	imageProcessingModule1->modelEnginePath = globalPath.modelPath;
	imageProcessingModule1->index = 1;
	imageProcessingModule1->BuildModule();

	imageProcessingModule2 = std::make_unique<ImageProcessingModuleWetPapers>(num, this);
	imageProcessingModule2->modelEnginePath = globalPath.modelPath;
	imageProcessingModule2->index = 2;
	imageProcessingModule2->BuildModule();
}

void ImgProModule::destroyImageProcessingModule()
{
	imageProcessingModule1.reset();
	imageProcessingModule2.reset();
}

void ImgProModule::onUpdateImgProContext()
{
	buildImgProContext();
	resetImgProIsUpdate(true);
}
