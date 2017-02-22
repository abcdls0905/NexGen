
#include "render.h"
#include "context.h"

#include "../public/core_log.h"
#include "../public/core_mem.h"
#include "../visual/vis_utils.h"

#include "../public/portable.h"

#include "../public/core_mem.h"

Render::Render()
{

}

Render::~Render()
{

}

bool Render::Init()
{
	return true;
}

bool Render::Shut()
{
	return true;
}

bool Render::NeedExecPerFrame()
{
	return true;
}

void Render::ExecFrameBegin()
{

}

void Render::ExecFrameEnd()
{

}

void Render::SetDeviceSize(int width, int height)
{

}

int Render::GetDeviceWidth()
{
	return 0;
}

int Render::GetDeviceHeight()
{
	return 0;
}

IColorRT* Render::CreateColorRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type)
{
	return NULL;
}

IDepthRT* Render::CreateDepthRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type)
{
	return NULL;
}

DeviceCaps* Render::GetDeviceCaps()
{
	return NULL;
}

ITexture* Render::CreateCubeTexture(const char* name, bool async)
{
	return NULL;
}

PERSISTID Render::GetDeviceCapsID() const
{
	return m_DeviceCapsID;
}

IDepthTex* Render::CreateDepthTex(unsigned int width, unsigned int height, TEX_FORMAT format, int type)
{
	return NULL;
}

IDepthStencilRT* Render::CreateDepthStencilRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type)
{
	return NULL;
}

IStencilTex* Render::CreateStencilTex(unsigned int width, unsigned int height, TEX_FORMAT format, int type)
{
	return NULL;
}
