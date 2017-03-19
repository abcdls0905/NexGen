
#include "../public/module.h"
#include "terrain.h"
#include "../visual/i_vis_base.h"
#include "../visual/i_texture.h"
#include "../public/var_list.h"
#include "../public/core_log.h"
#include "../visual/i_render.h"
#include "../visual/i_context.h"

DECLARE_ENTITY(Terrain, 0, IVisBase);

Terrain::Terrain()
{
  m_pRender = NULL;
  m_pContext = NULL;
}

Terrain::~Terrain()
{

}

bool Terrain::Init(const IVarList& args)
{
  m_pRender = (IRender*)GetCore()->GetInterface("Render");

  if (NULL == m_pRender)
  {
    return false;
  }

  return true;
}

bool Terrain::Shut()
{
  SAFE_RELEASE(m_pRender);
  return true;
}

void Terrain::SetContext(IRenderContext* value)
{
  Assert(value != NULL);
  m_pContext = value;
  // 设置自身为场景唯一对象
  m_pContext->SetUnique(this);
}

IRenderContext* Terrain::GetContext() const
{
  return m_pContext;
}

void Terrain::Update(float seconds)
{

}

void Terrain::Realize()
{
  int render_size = m_Visuals.size();
  for (size_t i = 0; i < render_size; ++i)
  {
    IVisBase* pVisBase = m_Visuals[i];
    pVisBase->Realize();
  }
}

bool Terrain::AddVisualRole(const char* name, const PERSISTID& id)
{
  IVisBase* pVisBase = (IVisBase*)GetCore()->GetEntity(id);
  if (NULL == pVisBase)
    return false;
  if (!pVisBase->GetEntInfo()->IsKindOf("IVisBase"))
    return false;
  return AddVisBase(name, pVisBase, true, 0.0F) == RESULT_SUCCESS;
}

bool Terrain::AddVisual(const char* name, const PERSISTID& id)
{
  IVisBase* pVisBase = (IVisBase*)GetCore()->GetEntity(id);
  if (NULL == pVisBase)
    return false;
  if (!pVisBase->GetEntInfo()->IsKindOf("IVisBase"))
    return false;
  return AddVisBase(name, pVisBase, true, 0.0F) == RESULT_SUCCESS;
}

int Terrain::AddVisBase(const char* name, IVisBase* pVisBase, bool is_role, float clip_radius)
{
  Assert(pVisBase != NULL);
  m_Visuals.push_back(pVisBase);
  return RESULT_SUCCESS;
}
