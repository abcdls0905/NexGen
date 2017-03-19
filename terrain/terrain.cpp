
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
