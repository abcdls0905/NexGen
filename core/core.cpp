
#include "core.h"
#include "../public/var_list.h"
#include "../public/inlines.h"
#include "../public/portable.h"
#include "../public/i_int_creator.h"
#include "../public/i_ent_creator.h"

ASSERTION_FAIL_DEFINED

// 核心指针
ICore* g_pCore = NULL;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//写入一行日志
bool fm_tracelog(const char * info, LOG_TYPE log_type)
{
	return true;
}


//读取一行日志
char* fm_logread(LOG_TYPE log_type)
{
	return 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus

void* CCore::MemoryCheckAllocDebug(size_t size, const char* file, int line)
{
	return NULL;
}

void* CCore::MemoryCheckAlloc(size_t size)
{
	return NULL;
}

void CCore::MemoryCheckFree(void* ptr, size_t size)
{

}

void* CCore::MemoryAlloc(size_t size)
{
	return NULL;
}

void CCore::MemoryFree(void* ptr)
{

}

CCore::CCore()
{
	g_pCore = this;
}

CCore::~CCore()
{
	g_pCore = NULL;
}
