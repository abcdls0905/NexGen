
#include "render_thread.h"

#include "../public/core_mem.h"
#include "../public/inlines.h"
#include "../public/portable.h"


CRenderThread::CRenderThread()
{
	m_pThread = (CThread*)CORE_ALLOC(sizeof(CThread));
	new (m_pThread)CThread(WorkerProc, InitWorkerProc, NULL, this, -1, 0);
}

CRenderThread::~CRenderThread()
{
	CORE_DELETE(m_pThread);
}

bool CRenderThread::Initialize()
{
	m_pThread->Start();
	return true;
}

bool CRenderThread::Shutdown()
{
	m_pThread->Stop();
	return true;
}

void CRenderThread::Clear()
{

}

void CRenderThread::WorkerFunc()
{

}

bool CRenderThread::Execute()
{
	return true;
}

bool CRenderThread::InitWorkerProc(void* pParameter)
{
	return true;
}

void CRenderThread::WorkerProc(void* pParameter)
{
	//glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
}
