#pragma once


#include"Modules.hpp"

#define LOG_TRACE(...)    Modules::getInstance().logModule.trace(__VA_ARGS__)
#define LOG_DEBUG(...)    Modules::getInstance().logModule.debug(__VA_ARGS__)
#define LOG_INFO(...)     Modules::getInstance().logModule.info(__VA_ARGS__)
#define LOG_WARN(...)     Modules::getInstance().logModule.warn(__VA_ARGS__)
#define LOG_ERROR(...)    Modules::getInstance().logModule.error(__VA_ARGS__)
#define LOG_CRITICAL(...) Modules::getInstance().logModule.critical(__VA_ARGS__)

#define LOG_USER_TRACE(...)    Modules::getInstance().logModule.traceUserOperate(__VA_ARGS__)
#define LOG_USER_DEBUG(...)    Modules::getInstance().logModule.debugUserOperate(__VA_ARGS__)
#define LOG_USER_INFO(...)     Modules::getInstance().logModule.infoUserOperate(__VA_ARGS__)
#define LOG_USER_WARN(...)     Modules::getInstance().logModule.warnUserOperate(__VA_ARGS__)
#define LOG_USER_ERROR(...)    Modules::getInstance().logModule.errorUserOperate(__VA_ARGS__)
#define LOG_USER_CRITICAL(...) Modules::getInstance().logModule.criticalUserOperate(__VA_ARGS__)

#define LOG_PLC_TRACE(...)    Modules::getInstance().logModule.tracePLC(__VA_ARGS__)
#define LOG_PLC_DEBUG(...)    Modules::getInstance().logModule.debugPLC(__VA_ARGS__)
#define LOG_PLC_INFO(...)     Modules::getInstance().logModule.infoPLC(__VA_ARGS__)
#define LOG_PLC_WARN(...)     Modules::getInstance().logModule.warnPLC(__VA_ARGS__)
#define LOG_PLC_ERROR(...)    Modules::getInstance().logModule.errorPLC(__VA_ARGS__)
#define LOG_PLC_CRITICAL(...) Modules::getInstance().logModule.criticalPLC(__VA_ARGS__)

#define LOG_CONNECT_STATUS_TRACE(...)    Modules::getInstance().logModule.traceConnectStatus(__VA_ARGS__)
#define LOG_CONNECT_STATUS_DEBUG(...)    Modules::getInstance().logModule.debugConnectStatus(__VA_ARGS__)
#define LOG_CONNECT_STATUS_INFO(...)     Modules::getInstance().logModule.infoConnectStatus(__VA_ARGS__)
#define LOG_CONNECT_STATUS_WARN(...)     Modules::getInstance().logModule.warnConnectStatus(__VA_ARGS__)
#define LOG_CONNECT_STATUS_ERROR(...)    Modules::getInstance().logModule.errorConnectStatus(__VA_ARGS__)
#define LOG_CONNECT_STATUS_CRITICAL(...) Modules::getInstance().logModule.criticalConnectStatus(__VA_ARGS__)

