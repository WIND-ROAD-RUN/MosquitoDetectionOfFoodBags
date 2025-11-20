#include "EliminateModule.hpp"

bool EliminateModule::build()
{
	detachDefectThread = std::make_unique<DetachDefectThreadWetPapers>(this);
	productPriorityQueue1 = std::make_unique<rw::dsl::ThreadSafeHeap<float, rw::dsl::HeapType::MinHeap>>(50);
	productPriorityQueue2 = std::make_unique<rw::dsl::ThreadSafeHeap<float, rw::dsl::HeapType::MinHeap>>(50);
	return true;
}

void EliminateModule::destroy()
{
	if (detachDefectThread)
	{
		detachDefectThread.reset();
	}
	if (productPriorityQueue1)
	{
		productPriorityQueue1.reset();
	}
	if (productPriorityQueue2)
	{
		productPriorityQueue2.reset();
	}
}

void EliminateModule::start()
{
	if (detachDefectThread)
	{
		detachDefectThread->startThread();
	}
}

void EliminateModule::stop()
{
	if (detachDefectThread)
	{
		detachDefectThread->stopThread();
		detachDefectThread->wait();
	}
}
