#include "DetachDefectThread.h"
#include "GlobalStruct.hpp"
#include <chrono>

#include "Modules.hpp"

DetachDefectThreadWetPapers::DetachDefectThreadWetPapers(QObject* parent)
{

}

DetachDefectThreadWetPapers::~DetachDefectThreadWetPapers()
{
	stopThread();
	wait(); // 等待线程安全退出
}

void DetachDefectThreadWetPapers::startThread()
{
	running = true;
	if (!isRunning()) {
		start(); // 启动线程
	}
}

void DetachDefectThreadWetPapers::stopThread()
{
	running = false; // 停止线程
}

void DetachDefectThreadWetPapers::processQueue1(std::unique_ptr<rw::dsl::ThreadSafeHeap<float>>& queue)
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
			auto cfg = Modules::getInstance().configManagerModule.setConfig;
			if (!cfg.isTichuDongzuo)
			{
				return;
			}
			std::cout << "pop1: " << static_cast<int>(nowLocation) << std::endl;

			auto isSuccess = zmotion->SetIOOut(1, ControlLines::tifeixinhaoOut, true, 100);
		}
	}
	catch (const std::runtime_error&)
	{
	}
}

void DetachDefectThreadWetPapers::processQueue2(std::unique_ptr<rw::dsl::ThreadSafeHeap<float>>& queue)
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
			auto cfg = Modules::getInstance().configManagerModule.setConfig;
			if (!cfg.isTichuDongzuo)
			{
				return;
			}
			auto isSuccess = zmotion->SetIOOut(1, ControlLines::tifeixinhaoOut, true, 100);
		}
	}
	catch (const std::runtime_error&)
	{

		return;
	}
}

void DetachDefectThreadWetPapers::run()
{
	auto& globalThread = GlobalThread::getInstance();
	auto& priorityQueue1 = Modules::getInstance().eliminateModule.productPriorityQueue1;
	auto& priorityQueue2 = Modules::getInstance().eliminateModule.productPriorityQueue2;
	while (running) {
		QThread::msleep(1);

		processQueue1(priorityQueue1);
		processQueue2(priorityQueue2);
	}
}
