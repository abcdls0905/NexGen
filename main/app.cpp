
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __ANDROID__
#include <android/log.h>
#define  LOG_TAG    "fm"
#endif

#include "app.h"
#include "../public/i_core.h"

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

void App::Init(const char* workpath, const IVarList& args, int screen_width, int screen_height, float scale)
{

}

void App::OnFrame()
{

}

void App::OnRender()
{

}

void App::Shutdown()
{

}

bool App::OnMsg(unsigned int msg, size_t param1, size_t param2, int& result)
{
	return true;
}

void App::OnInput(const char* utf8)
{

}

