#pragma once

#include <qobject.h>

#include "oso_StorageContext.hpp"
#include "rqw_ImageSaveEngine.h"
#include "RuntimeInfoModule.hpp"
#ifdef BUILD_WITHOUT_HARDWARE
#include "TestImgGenerated.hpp"
#endif

class GlobalThread
	:public QObject
{
	Q_OBJECT
public:
	static GlobalThread& getInstance();

	GlobalThread(const GlobalThread&) = delete;
	GlobalThread& operator=(const GlobalThread&) = delete;
private:
	GlobalThread();
	~GlobalThread() override;
#ifdef BUILD_WITHOUT_HARDWARE
public:
	DetachTestImgThread* detachTestImgThread{ nullptr };
	void build_DetachTestImgThread();
	void destroy_DetachTestImgThread();
#endif
};