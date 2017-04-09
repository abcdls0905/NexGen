
#include "actor.h"
#include "../visual/i_context.h"
#include "../visual/vis_utils.h"
#include "../public/var_list.h"
#include "../public/i_ini_file.h"
#include "../public/inlines.h"
#include "../public/core_log.h"
#include "../public/core_mem.h"
#include "../public/core_file.h"
#include "../public/portable.h"
#include "../model/model_data.h"


#define performance_time Port_GetPerformanceTime

DECLARE_ENTITY(Actor,0, IVisBase);

Actor::Actor()
{
  m_pRender = NULL;
  m_pModelSystem = NULL;
  m_pModelPlayer = NULL;
  m_bVisible = true;
  FmMatrixIdentity(&m_mtxWorld);
}

Actor::~Actor()
{

}

bool Actor::Init(const IVarList& args)
{
  m_pRender = (IRender*)GetCore()->GetInterface("Render");
  if (NULL == m_pRender)
    return false;
  m_pModelSystem = (IModelSystem*)GetCore()->GetInterface("ModelSystem");
  if (NULL == m_pModelSystem)
    return false;
  return true;
}

bool Actor::Shut()
{
  return true;
}

void Actor::Load(const char* modelfile, const char* type)
{
  Assert(m_pModelSystem != NULL);
  Assert(modelfile != NULL);
  m_ConfigFile = modelfile;

  char fname[256];
  SafeSprintf(fname, sizeof(fname), "%s%s", 
    m_pRender->GetPathPrefix(modelfile), modelfile);

  //load_model(fname, type);

  m_pModelPlayer = m_pModelSystem->CreateModelPlayer(fname,
    GetCore()->GetTotalSeconds(), "", false);
}

void Actor::Realize()
{
  if (!m_bVisible)
    return;
  if (!m_pModelPlayer)
    return;
  m_pModelPlayer->SetWorldMatrix(m_mtxWorld);
  // 获取当前视图模式
  ISceneView::SCENE_VIEW_TYPE scene_view_type = m_pRender->GetSceneView()->GetSceneViewType();
  switch(scene_view_type)
  {
  case ISceneView::TYPE_NORMAL:
    RealizeNormal();
    break;
  case ISceneView::TYPE_REFLECTION:
    RealizeReflect();
    break;
  default:
    break;
  }
}

void Actor::RealizeNormal()
{
#ifdef PERFORMANCE_DEBUG
  double dRealizeModel = performance_time();
#endif
  IRenderContext* pContext = m_pRender->GetContext();
//   m_bShowBoundBox = true;
//   // 设置模型颜色
//   m_pModelPlayer->SetColor(m_nColor);
//   m_pModelPlayer->SetUnderWater(m_bUnderWater);
//   m_pModelPlayer->SetShowBoundBox(m_bShowBoundBox);
   m_pModelPlayer->Draw(pContext->GetClipPlanes(), pContext->GetClipPlaneAmount());

#ifdef PERFORMANCE_DEBUG
  m_pRender->GetPerformance()->dModelRealizeTime +=
    performance_time() - dRealizeModel;
#endif
}

void Actor::RealizeReflect()
{

}

PERSISTID Actor::GetLinkObject(const char* name)
{
  return PERSISTID();
}

bool Actor::SetPosition(float x, float y, float z)
{
  Assert(m_pModelSystem != NULL);

  m_mtxWorld._41 = x;
  m_mtxWorld._42 = y;
  m_mtxWorld._43 = z;
  return true;
}

FmVec3 Actor::GetPosition() const
{
  return FmVec3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}

bool Actor::SetAngle(float x, float y, float z)
{
  VisUtil_SetMatrixAngle(&m_mtxWorld, x, y, z);
  return true;
}

FmVec3 Actor::GetAngle() const
{
  FmVec3 angle;
  VisUtil_GetMatrixAngle(&angle, &m_mtxWorld);
  return angle;
}

void Actor::RealizeShadowMap()
{
  UpdateMatrix(NULL);

  IRenderContext* pContext = m_pRender->GetContext();
  m_pModelPlayer->DrawShadowMap(pContext->GetClipPlanes(), pContext->GetClipPlaneAmount());
}

void Actor::UpdateMatrix(IVisBase* pVisBase)
{

}
