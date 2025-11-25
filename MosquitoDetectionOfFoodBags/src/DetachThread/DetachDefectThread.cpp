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

			auto isSuccess = zmotion->SetIOOut(1, ControlLines::tifeixinhaoOut, true, 100);
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
