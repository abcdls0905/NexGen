
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __ANDROID__
#include <android/log.h>
#define  LOG_TAG    "fm"
#endif

#include "app.h"
#include "../game/global.h"
#include "../public/i_core.h"
#include "../visual/i_world.h"
#include "../public/core_config.h"
#include "../utils/share_mem.h"

#ifdef FX_SYSTEM_WINDOWS
#include "../system/win_encoding.h"
#include "../system/win_string.h"
#endif // FX_SYSTEM_WINDOWS

#ifdef FX_SYSTEM_LINUX
#include "../system/linux_encoding.h"
#include "../system/linux_string.h"
#endif // FX_SYSTEM_LINUX

#ifdef FX_SYSTEM_IOS
#include "../system/ios_encoding.h"
#include "../system/ios_string.h"
#endif // FX_SYSTEM_IOS


#define UI_WIDTH 1024
#define UI_HEIGHT 768

extern void lib_main_world();
extern void lib_main_render();
extern void lib_main_game();
extern void lib_main_model();
extern void lib_main_terrain();
extern void lib_main_gui();

// 加载核心配置文件
static bool load_core_config(CCoreConfig& config)
{
	// 主循环休眠时间
	config.m_nSleep = 0;
	config.m_strProgramPath = config.m_strWorkPath + "pro\\";
	config.m_strResourcePath = config.m_strWorkPath + "res\\";
	// 文件系统
	config.m_strFileSystem = "";
	// 文件系统配置信息
	config.m_strFileConfig = "config.cfg";
	// 主实体名
	config.m_strMainEntity = "World";
	return true;
}
// 显示错误对话框
static void show_fatal_error(const char* info)
{
#ifdef _WIN32
	MessageBoxA(NULL, info, "fatal error", MB_OK | MB_ICONERROR);
#endif
}

void App::Init(const char* workpath, const IVarList& args, int screen_width, int screen_height, float scale)
{
  lib_main_render();
	lib_main_world();
	lib_main_game();
  lib_main_model();
  lib_main_terrain();
  lib_main_gui();

	ICore* core = NULL;
	IWorld* pWorld = NULL;

	// 核心配置
	CCoreConfig config;

	config.m_strWorkPath = workpath;
	//	config.m_nFlags |= CORE_FLAG_MEMORY_VALIDATE;

	if (!load_core_config(config))
	{
		show_fatal_error("load_core_config failed");
		abort();
	}

	// 核心初始化
	core = core_initialize(config);
	if (!core)
	{
		show_fatal_error("core_initialize failed");
		abort();
	}
	// 启动核心前必须设置
	g_core = core;
	// 启动核心
	if (!core_startup(core, args))
	{
		show_fatal_error("Core startup failed");
		abort();
	}
	pWorld = (IWorld*)core->GetMainEntity();
 	pWorld->SetWidth(screen_width);
 	pWorld->SetHeight(screen_height);
 	pWorld->CreateDevice();
 	g_world = pWorld;

	g_core->CreateEntityArgs("Game", CVarList() << UI_WIDTH << UI_HEIGHT << screen_height / float(UI_HEIGHT));

	// 进程索引值
#ifdef _WIN32    
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
#endif
	m_timer.SetUseQPF(true);
	m_timer.Initialize();
	m_initok = true;
}

void App::OnFrame()
{
	g_world->Display(0.0F);
}

void App::OnRender()
{
	double time = m_timer.GetElapseTime();
	g_core->FrameCheckMessage();
	g_core->FrameExecute(time);
}

void App::Shutdown()
{

}

bool App::OnMsg(unsigned int msg, size_t param1, size_t param2, int& result)
{
  return g_core->ProcessMessage(msg, param1, param2, result);
}

void App::OnInput(const char* utf8)
{

}

