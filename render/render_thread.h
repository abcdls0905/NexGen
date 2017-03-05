
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

	// ��ʼ��
	bool Initialize();
	// ����
	bool Shutdown();
	// ���������������
	void Clear();
	// ��������
	void WorkerFunc();
	// ��ȡ�������
	bool Execute();
	// �������
};

#endif