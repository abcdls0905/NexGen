
#ifndef _RENDER_THREAD_H_
#define _RENDER_THREAD_H_

#include "../public/macros.h"
#include "../public/persistid.h"
#include "../public/core_mem.h"
#include "../utils/lock_util.h"
#include "../utils/thread.h"

class CRenderThread
{
private:
	CThread* m_pThread;	
	static bool InitWorkerProc(void* pParameter);
	static void WorkerProc(void* pParameter);

public:
	CRenderThread();
	~CRenderThread();

	// 初始化
	bool Initialize();
	// 结束
	bool Shutdown();
	// 清除所有输入和输出
	void Clear();
	// 处理输入
	void WorkerFunc();
	// 读取结果数据
	bool Execute();
	// 请求加载
};

#endif