#include "GlobalStruct.hpp"

GlobalThread& GlobalThread::getInstance()
{
	static GlobalThread instance;
	return instance;
}

GlobalThread::GlobalThread()
{
}

GlobalThread::~GlobalThread()
{
}

#ifdef BUILD_WITHOUT_HARDWARE
void GlobalThread::build_DetachTestImgThread()
{
	detachTestImgThread = new DetachTestImgThread(this);
}

void GlobalThread::destroy_DetachTestImgThread()
{
	if (detachTestImgThread)
	{
		detachTestImgThread->stopThread();
		detachTestImgThread->wait();
		delete detachTestImgThread;
		detachTestImgThread = nullptr;
	}
}

#endif
