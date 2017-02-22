
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

// 工作路径
char g_strWorkPath[256] = { 0 };

// 初始窗口大小
const DWORD FACE_WIDTH = 1280;
const DWORD FACE_HEIGHT = 768;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
 	case WM_CLOSE:
		//设置推出
	 	DestroyWindow(hwnd);
		// 防止系统自动关闭窗口
 		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

    // 解除F10或Alt键锁屏功能
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
		//work_path = "D:\\散包更新器\\voyage\\";
		//work_path = "I:\\work\\MiniClient\\Res\\client\\";
		//work_path = "C:\\amber_client\\";
#endif
		
		// 创建Windows窗口
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
		// 启动参数
		CVarList args;
		App::Inst().Init(work_path.c_str(), args, winRect.right - winRect.left, winRect.bottom - winRect.top, 1.0f);
	}

	// 进程索引值
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

	// 主线程与CPU核心绑定以保证计时器的正确性
	SetThreadAffinityMask(GetCurrentThread(), mask);

	// 计时器
	CGameTimer timer;

	timer.SetUseQPF(true);
	timer.Initialize();*/

	// 主消息循环
	MSG	msg;
	
	for (;;)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message) 
			{
				// 核心关闭
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

	// 核心关闭
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
