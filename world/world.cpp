
#include "world.h"
#include "../visual/i_vis_base.h"
#include "../visual/i_texture.h"
#include "../public/var_list.h"
#include "../public/core_log.h"
#include "../visual/i_render.h"
#include "../visual/i_context.h"

DECLARE_ENTITY(World, 0, IEntity);


World::World()
{
	m_nBackColor = 0x00000000;
	m_nWidth = 800;
	m_nHeight = 600;
	m_pRender = NULL;
}

World::~World()
{

}

bool World::Init(const IVarList& args)
{
	return true;
}

bool World::Shut()
{
	return true;
}

void World::Execute(float seconds)
{

}

void World::Display(float offset_seconds)
{
	m_pRender->BeginFrame(m_nBackColor);
	m_pRender->EndFrame();
}

void World::SetWidth(int value)
{
	m_nWidth = value;
}

int World::GetWidth() const
{
	return m_nWidth;
}

void World::SetHeight(int value)
{
	m_nHeight = value;
}

int World::GetHeight() const
{
	return m_nHeight;
}

bool World::CreateDevice(bool multithreading)
{
	m_pRender = (IRender*)GetCore()->GetInterface("Render");
	if (NULL == m_pRender)
	{
		CORE_TRACE("(World::CreateDevice)render not found");
		return false;
	}
	if (!m_pRender->CreateDevice(multithreading))
	{
		return false;
	}
	return true;
}

void World::SetBackColor(const char* value)
{
	m_nBackColor = VisUtil_ColorToInt(value);
}
