#include "DetachDefectThread.h"
#include <chrono>
#include "Modules.hpp"

DetachDefectThreadMDOFoodBags::DetachDefectThreadMDOFoodBags(QObject* parent)
{

}

DetachDefectThreadMDOFoodBags::~DetachDefectThreadMDOFoodBags()
{
	stopThread();
	wait(); // 等待线程安全退出
}

void DetachDefectThreadMDOFoodBags::startThread()
{
	running = true;
	if (!isRunning()) {
		start(); // 启动线程
	}
}

void DetachDefectThreadMDOFoodBags::stopThread()
{
	running = false; // 停止线程
}

void DetachDefectThreadMDOFoodBags::processQueue1(std::unique_ptr<rw::dsl::ThreadSafeHeap<float>>& queue)
{

	auto& zmotion = Modules::getInstance().motionControllerModule.zmotion;
	auto& setConfig = Modules::getInstance().configManagerModule.setConfig;
	try
	{
		float nowLocation = 0;
		bool isget = false;
		nowLocation = zmotion->getAxisLocation(0, isget);

		float minlocation = 0;

		queue->tryGetTop(minlocation);
		if (!minlocation)
		{
			return;
		}

		if (((nowLocation)-(minlocation)) >= 0)
		{
			queue->tryPopTop(nowLocation);
			auto& mainWindowConfig = Modules::getInstance().configManagerModule.MainWindowsConfig;
			auto& runtimeInfoModule = Modules::getInstance().runtimeInfoModule;

		    auto isSuccess = zmotion->SetIOOut(1, ControlLines::tifeixinhaoOut, true, 100);
			isSuccess = zmotion->SetIOOut(2, ControlLines::baojingOut, true, static_cast<int>(setConfig.baojingchixushijian));
		}
	}
	catch (const std::runtime_error&)
	{
	}
}

void DetachDefectThreadMDOFoodBags::run()
{
	auto& priorityQueue1 = Modules::getInstance().eliminateModule.productPriorityQueue1;
	while (running) {
		QThread::msleep(1);

		processQueue1(priorityQueue1);
	}
}
