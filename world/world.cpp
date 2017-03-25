
#include "world.h"
#include "scene.h"
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
  m_pMainScene = NULL;
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
#ifdef PERFORMANCE_DEBUG
  double dRealizeTime = performance_time();
#endif
  // ÏÈäÖÈ¾³¡¾°
  if (m_pMainScene)
  {
    m_pMainScene->Realize();
  }
#ifdef PERFORMANCE_DEBUG
  m_pRender->GetPerformance()->dRealizeTime += 
    performance_time() - dRealizeTime;
#endif
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
  m_pRender->SetDeviceSize(m_nWidth, m_nHeight);

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

void World::SetMainSceneID(const PERSISTID& id)
{
  if (id.IsNull())
  {
    m_pMainScene = NULL;
    return;
  }

  IEntity* pEntity = GetCore()->GetEntity(id);

  if (NULL == pEntity)
  {
    return;
  }

  m_pMainScene = (Scene*)pEntity;
}

PERSISTID World::GetMainSceneID() const
{
  if (NULL == m_pMainScene)
  {
    return PERSISTID();
  }
  return m_pMainScene->GetID();
}
