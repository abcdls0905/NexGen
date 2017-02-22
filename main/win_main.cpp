
//#define _CONSOLE
#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <stdio.h>
#include "../public/macros.h"
#include "../public/i_core.h"
#include "../public/fast_str.h"
#include "../public/var_list.h"
#include "../public/inlines.h"

#include "win_graphdevice.h"
#include "app.h"

// ����·��
char g_strWorkPath[256] = { 0 };

// ��ʼ���ڴ�С
const DWORD FACE_WIDTH = 1280;
const DWORD FACE_HEIGHT = 768;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
 	case WM_CLOSE:
		//�����Ƴ�
	 	DestroyWindow(hwnd);
		// ��ֹϵͳ�Զ��رմ���
 		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

    // ���F10��Alt����������
    if (iMsg == WM_SYSKEYUP && (wParam == VK_MENU || wParam == VK_F10))
    {
	    return 0L;
    }

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

int WinMainProc(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, 
	int iCmdShow)
{
#ifdef _CONSOLE
  AllocConsole();
  freopen("CONOUT$","w+t",stdout);
  freopen("CONIN$","r+t",stdin);
#endif
	CGraphDevice graphedevice;
	if (true)
	{
#ifndef _DEBUG_RT_
		//work_path = "D:\\ɢ��������\\voyage\\";
		//work_path = "I:\\work\\MiniClient\\Res\\client\\";
		//work_path = "C:\\amber_client\\";
#endif
		
		// ����Windows����
		WNDCLASSEX wndclass;

		wndclass.cbSize = sizeof(wndclass);
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;//sizeof(WORD);
		wndclass.hInstance = hInstance;
		wndclass.hIcon = NULL;
		wndclass.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = TEXT("engine");
		wndclass.hIconSm = NULL;
		
		RegisterClassEx(&wndclass);

		RECT winRect;
		SetRect(&winRect,0,0,FACE_WIDTH,FACE_HEIGHT);
		AdjustWindowRectEx(&winRect, WS_CAPTION|WS_SYSMENU, false, 0);

		int x =  -winRect.left;
		winRect.left += x;
		winRect.right += x;
		int y =  -winRect.top;
		winRect.top += y;
		winRect.bottom += y;

		//WS_VISIBLE|WS_CAPTION|WS_SYSMENU
		HWND hwnd = CreateWindow(TEXT("engine"), TEXT("engine"), WS_OVERLAPPEDWINDOW,
			(GetSystemMetrics(SM_CXSCREEN) - FACE_WIDTH) / 2, 
			(GetSystemMetrics(SM_CYSCREEN) - FACE_HEIGHT) / 2, 
			winRect.right-winRect.left, winRect.bottom-winRect.top, NULL, NULL, hInstance, NULL);

		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
		::GetClientRect(hwnd, &winRect);

		graphedevice.InitDevice(hwnd);
		result_string work_path = "";
		// ��������
		CVarList args;
		App::Inst().Init(work_path.c_str(), args, winRect.right - winRect.left, winRect.bottom - winRect.top, 1.0f);
	}

	// ��������ֵ
	/*
	CShareMem share_mem;
	bool exists = false;

	share_mem.Create("fx_main_share_process_count", sizeof(unsigned int), 
		&exists);

	unsigned int* pProcessCount = (unsigned int*)share_mem.GetMem();
	unsigned int process_index;

	if (exists)
	{
		process_index = *pProcessCount;
	}
	else
	{
		process_index = 0;
	}

	*pProcessCount = process_index + 1;

	SYSTEM_INFO si;

	GetSystemInfo(&si);

	unsigned int mask = 0x1 << (process_index % si.dwNumberOfProcessors);

	// ���߳���CPU���İ��Ա�֤��ʱ������ȷ��
	SetThreadAffinityMask(GetCurrentThread(), mask);

	// ��ʱ��
	CGameTimer timer;

	timer.SetUseQPF(true);
	timer.Initialize();*/

	// ����Ϣѭ��
	MSG	msg;
	
	for (;;)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message) 
			{
				// ���Ĺر�
				graphedevice.ShutDevice();
				return 1;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
		}

		App::Inst().OnFrame();
		App::Inst().OnRender();
		graphedevice.Present();
	}

	// ���Ĺر�
	graphedevice.ShutDevice();
	return 1;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, int iCmdShow)
{

#ifdef _DEBUG
	// Enable memory-leak reports
 //	_CrtSetBreakAlloc(1911);
#endif
	if (IsDebuggerPresent() == FALSE)
	{
//		InitMiniDump();
	}
	return WinMainProc(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
}
