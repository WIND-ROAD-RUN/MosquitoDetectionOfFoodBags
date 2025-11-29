#include "CameraModule.hpp"

#include <QRegularExpression>
#include "Modules.hpp"
#include"rqw_CameraObjectThread.hpp"


CameraModule::CameraModule()
{
}

CameraModule::~CameraModule()
{
}

std::vector<BuildError> CameraModule::build()
{
	std::vector<BuildError> errorList;
	if (!build_camera1())
	{
		errorList.emplace_back(Camera1Error);
	}
	_buildResults = errorList;
	return std::vector<BuildError>();
}

void CameraModule::destroy()
{
	destroy_camera1();
}

void CameraModule::start()
{
	if (camera1)
	{
		camera1->startMonitor();
	}
}

void CameraModule::stop()
{
	if (camera1)
	{
		camera1->stopMonitor();
	}
}

bool CameraModule::build_camera1()
{
	auto cameraList = rw::rqw::CheckCameraList(rw::rqw::CameraProvider::DS);

	auto cameraMetaData1 = cameraMetaDataCheck(Utility::cameraIp1, cameraList);

	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	double xiangsudangliang= setConfig.xiangSuDangLiang1;
	double xiangjichufachangdu = setConfig.xiangjichufachangdu;

	if (cameraMetaData1.ip != "0")
	{
		try
		{
			camera1 = std::make_unique<rw::rqw::CameraPassiveThread>(this);
			// 相机触发回调
			camera1->callBackForImgReadyBefore = [](rw::rqw::MatInfo& matInfo) {
					auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
					auto loc = zmotion->getModbus(2, 1);
					matInfo.customField["loc"] = static_cast<float>(loc);
				};
			camera1->initCamera(cameraMetaData1, rw::rqw::CameraObjectTrigger::Hardware);
			//camera1->setTriggerState(true);
			camera1->cameraIndex = 1;
			camera1->setFrameTriggered(false);
			camera1->setLineTriggered(true);

			//TODO:设置相机行高
			double hanggao = xiangjichufachangdu / xiangsudangliang;
			
			camera1->setLineHeight((int)hanggao);
			camera1->setExposureTime(static_cast<size_t>(setConfig.baoguang1));
			camera1->setGain(static_cast<size_t>(setConfig.zengyi1));
			camera1->setMultiplier(20);
			camera1->setPostDivider(19);

			QObject::connect(camera1.get(), &rw::rqw::CameraPassiveThread::frameCaptured,
				this, &CameraModule::onFrameCaptured);

			return true;
		}
		catch (const std::exception&)
		{ 
			return false;
		}
	}
	return false;
}

void CameraModule::destroy_camera1()
{
	camera1.reset();
}

bool CameraModule::isTargetCamera(const QString& cameraIndex, const QString& targetName)
{
	QRegularExpression regex(R"((\d+)\.(\d+)\.(\d+)\.(\d+))");
	QRegularExpressionMatch match = regex.match(targetName);

	if (match.hasMatch()) {
		auto matchString = match.captured(3);

		return cameraIndex == matchString;
	}

	return false;
}

rw::rqw::CameraMetaData CameraModule::cameraMetaDataCheck(const QString& cameraIndex,
	const QVector<rw::rqw::CameraMetaData>& cameraInfo)
{
	for (const auto& cameraMetaData : cameraInfo) {
		if (isTargetCamera(cameraIndex, cameraMetaData.ip)) {
			return cameraMetaData;
		}
	}
	rw::rqw::CameraMetaData error;
	error.ip = "0";
	return error;
}

bool CameraModule::onBuildCamera(int index)
{
	switch (index)
	{
	case 1:
		return build_camera1();
	default:
		return false;
	}
}

void CameraModule::onDestroyCamera(int index)
{
	switch (index)
	{
	case 1:
		destroy_camera1();
		break;
	case 2:
		break;
	default:
		break;
	}
}

void CameraModule::onStartCamera(int index)
{
	switch (index)
	{
	case 1:
		if (camera1)
		{
			camera1->startMonitor();
		}
		break;
	default:
		break;
	}
}

void CameraModule::onFrameCaptured(rw::rqw::MatInfo frame, size_t index)
{
	float loc = 0;
	if (frame.customField.find("loc")!= frame.customField.end())
	{
		loc = std::any_cast<float>(frame.customField["loc"]);
	}
	switch (index)
	{
		case 1:
			emit frameCaptured1(frame, index,loc);
			break;
		default:
			break;
	}
}
