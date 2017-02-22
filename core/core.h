
#ifndef _CORE_H
#define _CORE_H

#include "../public/macros.h"
#include "../public/i_core.h"

class CCore : public ICore
{
public:
	CCore();
	virtual ~CCore();

public:
	// 跨模块内存管理
	virtual void* MemoryCheckAllocDebug(size_t size, const char* file,
		int line);
	virtual void* MemoryCheckAlloc(size_t size);
	virtual void MemoryCheckFree(void* ptr, size_t size);
	virtual void* MemoryAlloc(size_t size);
	virtual void MemoryFree(void* ptr);
};

#endif