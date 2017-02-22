
#ifndef _PUBLIC_I_CORE_H
#define _PUBLIC_I_CORE_H

#include "macros.h"
#include "persistid.h"
#include "i_var.h"
#include "i_var_list.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
//日志类型
enum LOG_TYPE
{
	LOG_RENDER,//渲染相关
	LOG_GUI,//GUi相关监控
	LOG_NET,//网络相关监控
	LOG_CORE,//Core相关监控
	LOG_PHYSICS,//物理监控
	LOG_LOGIC,//逻辑监控
	LOG_FILEIO,//文件IO监控
	LOG_FRAME,//渲染帧数据
	LOG_PERF,//性能超时相关监控
	LOG_TYPE_MAX,//最大监控类型
};

//写入一行日志 (支持1024个字节以内有效）
extern bool fm_tracelog(const char * info, LOG_TYPE log_type = LOG_LOGIC);

//读取一行日志
extern char* fm_logread(LOG_TYPE log_type = LOG_LOGIC);

#ifdef __cplusplus
}
#endif // __cplusplus

// 核心接口

class IEntity;
class IInterface;
class IEntInfo;

class ICore
{
public:
	virtual ~ICore() = 0;
	// 跨模块内存管理
	virtual void* MemoryCheckAllocDebug(size_t size, const char* file,
		int line) = 0;
	virtual void* MemoryCheckAlloc(size_t size) = 0;
	virtual void MemoryCheckFree(void* ptr, size_t size) = 0;
	virtual void* MemoryAlloc(size_t size) = 0;
	virtual void MemoryFree(void* ptr) = 0;

	// 删除实体
	virtual bool DeleteEntity(const PERSISTID& id) = 0;
	// 释放功能接口
	virtual void ReleaseInterface(IInterface* pInterface) = 0;
	// 获得实体
	virtual IEntity* GetEntity(const PERSISTID& id) = 0;
};

inline ICore::~ICore() {}

#endif