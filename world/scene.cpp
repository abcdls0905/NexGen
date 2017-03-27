
#include "scene.h"
#include "../visual/i_texture.h"
#include "../visual/i_vis_base.h"
#include "../visual/vis_utils.h"
#include "../public/core_log.h"
#include "../visual/i_render.h"
#include "../visual/i_context.h"

#include "pssm.h"

DECLARE_ENTITY(Scene, 0, IEntity);

Scene::Scene()
{
  m_pRender = NULL;
  m_pContext = NULL;
  m_pSceneView = NULL;
  m_pObjects = NULL;
  m_pCamera = NULL;
  m_pPSSM = NULL;
}

Scene::~Scene()
{
  object_t* p = m_pObjects;
  object_t* t;

  while (p)
  {
    t = p;
    p = p->pNext;
    CORE_FREE(t, sizeof(object_t));
  }
  SAFE_RELEASE(m_pContext);
  SAFE_RELEASE(m_pSceneView);
}

bool Scene::Init(const IVarList& args)
{
  m_pRender = (IRender*)GetCore()->GetInterface("Render");

  if (NULL == m_pRender)
  {
    return false;
  }

  // 创建渲染器参数
  m_pContext = m_pRender->CreateContext();
  // 创建场景视图
  m_pSceneView = m_pRender->CreateSceneView(ISceneView::TYPE_NORMAL,"MainSceneView");

  // 设置环境上下文
  m_pRender->SetContext(m_pContext);
  m_pRender->SetSceneView(m_pSceneView);
  return true;
}

bool Scene::Shut()
{
  object_t* p = m_pObjects;
  object_t* t;
  while (p)
  {
    t = p;
    p = p->pNext;

    IEntity* pEntity = GetCore()->GetEntity(t->ObjectId);

    SAFE_RELEASE(pEntity);
    CORE_FREE(t, sizeof(object_t));
  }
  m_pObjects = NULL;
  SAFE_RELEASE(m_pCamera);
  SAFE_RELEASE(m_pContext);
  SAFE_RELEASE(m_pRender);
  SAFE_RELEASE(m_pSceneView);
  DeletePSSM();
  return true;
}

void Scene::Update(float seconds)
{
  // 设置环境上下文
  m_pRender->SetContext(m_pContext);
  m_pRender->SetSceneView(m_pSceneView);

  bool bDynamicShadow = m_pContext->GetEnableDynamicShadow();

  if (bDynamicShadow && m_pPSSM)
  {
    m_pPSSM->SetRenderObjets(m_pObjects);
  }

  object_t* prev = NULL;
  object_t* p = m_pObjects;

  while (p)
  {
    IVisBase* pVisBase = (IVisBase*)GetCore()->GetEntity(p->ObjectId);

    if (pVisBase != NULL)
    {
      pVisBase->Update(seconds);
      prev = p;
      p = p->pNext;
    }
    else
    {
      object_t* t = p;

      p = p->pNext;
      CORE_FREE(t, sizeof(object_t));

      if (prev != NULL)
      {
        prev->pNext = p;
      }
      else
      {
        m_pObjects = p;
      }
    }
  }

  // 恢复环境上下文
  m_pRender->SetContext(NULL);
  m_pRender->SetSceneView(NULL);
}

void Scene::Realize()
{
  // 保存上一帧的摄像机参数
  m_pContext->SavePrevCamera();

  // 设置环境上下文
  m_pRender->SetContext(m_pContext);
  m_pRender->SetSceneView(m_pSceneView);

  m_pSceneView->SetContext(m_pContext);
  if(m_scenetype != IScene::SCENETYPE_GUI)
    m_pRender->AddSceneView(m_pSceneView);

  if (m_pCamera)
  {
    m_pCamera->Realize();
  }

  bool bDynamicShadow = m_pContext->GetEnableDynamicShadow();

  if (bDynamicShadow && m_pPSSM)
  {
    m_pContext->CalculateShadowMapMatrix();
  }

  object_t* p = m_pObjects;	
  while (p)
  {
    IVisBase* pVisBase = (IVisBase*)GetCore()->GetEntity(p->ObjectId);

    if (pVisBase)
    {
      pVisBase->Realize();
    }
    p = p->pNext;
  }

  if (bDynamicShadow && m_pPSSM)
  {
    m_pPSSM->GenerateShadowMap();
  }

  // 恢复环境上下文
  m_pRender->SetContext(NULL);
  m_pRender->SetSceneView(NULL);
}

bool Scene::AddObject(const PERSISTID& obj, int priority)
{
  IVisBase* pVisBase = GetVisBase(obj);

  if (NULL == pVisBase)
  {
    return false;
  }
  if (pVisBase->GetContext() != m_pContext)
  {
    CORE_TRACE("(Scene::AddObject)context not match");
    CORE_TRACE(pVisBase->GetEntInfo()->GetEntityName());
    return false;
  }
  object_t* pObject = (object_t*)CORE_ALLOC(sizeof(object_t));
  pObject->ObjectId = obj;
  pObject->nPrior = priority;
  // 获得优先级更高的节点
  object_t* p = m_pObjects;
  object_t* t = NULL;
  while (p)
  {
    // 相同级别先加入的在前面
    if (p->nPrior <= priority)
    {
      t = p;
    }
    p = p->pNext;
  }
  if (t == NULL)
  {
    // 排在最前
    pObject->pNext = m_pObjects;
    m_pObjects = pObject;
  }
  else
  {
    // 排在其后
    pObject->pNext = t->pNext;
    t->pNext = pObject;
  }

  return true;
}

IVisBase* Scene::GetVisBase(const PERSISTID& id) const
{
  IEntity* pEntity = GetCore()->GetEntity(id);
  if (NULL == pEntity)
  {
    return NULL;
  }
  if (!pEntity->GetEntInfo()->IsKindOf("IVisBase"))
  {
    return NULL;
  }
  return (IVisBase*)pEntity;
}

bool Scene::RemoveObject(const PERSISTID& obj)
{
  object_t* prev = NULL;
  object_t* p = m_pObjects;

  while (p)
  {
    if (p->ObjectId == obj)
    {
      if (prev != NULL)
      {
        prev->pNext = p->pNext;
      }
      else
      {
        m_pObjects = p->pNext;
      }

      CORE_FREE(p, sizeof(object_t));
      return true;
    }
    else
    {
      prev = p;
      p = p->pNext;
    }
  }

  return false;
}

bool Scene::RemoveObjectByPrior(int priority)
{
  object_t* prev = NULL;
  object_t* p = m_pObjects;

  while (p)
  {
    if (p->nPrior == priority)
    {
      if (prev != NULL)
      {
        prev->pNext = p->pNext;
      }
      else
      {
        m_pObjects = p->pNext;
      }

      CORE_FREE(p, sizeof(object_t));
      return true;
    }
    else
    {
      prev = p;
      p = p->pNext;
    }
  }

  return false;
}

PERSISTID Scene::Create(const char* name)
{
  IVisBase* pVisBase = (IVisBase*)GetCore()->CreateEntity(name);

  if (NULL == pVisBase)
  {
    return PERSISTID();
  }

  if (!pVisBase->GetEntInfo()->IsKindOf("IVisBase"))
  {
    pVisBase->Release();
    return PERSISTID();
  }

  // 设置当前场景上下文
  pVisBase->SetContext(m_pContext);

  return pVisBase->GetID();
}

PERSISTID Scene::GetCameraID()
{
  if (NULL == m_pCamera)
  {
    m_pCamera = (Camera*)GetCore()->CreateEntityArgs("Camera",
      CVarList() << this->GetID());
  }

  return m_pCamera->GetID();
}

PERSISTID Scene::GetPSSM() const
{
  if (NULL == m_pPSSM)
  {
    return PERSISTID();
  }

  return m_pPSSM->GetID();
}

PERSISTID Scene::CreatePSSM()
{
  if (NULL == m_pPSSM)
  {
    m_pPSSM = (PSSM*)(GetCore()->CreateEntity("PSSM"));
    m_pPSSM->Load();

    m_pPSSM->SetContext(m_pContext);
  }

  return m_pPSSM->GetID();
}

bool Scene::DeletePSSM()
{
  if (NULL == m_pPSSM)
  {
    return false;
  }

  SAFE_RELEASE(m_pPSSM);

  return true;
}

void Scene::SetEnableDynamicShadow(bool value)
{
  m_pContext->SetEnableDynamicShadow(value);

  if (value && (m_pPSSM == NULL))
  {
    CreatePSSM();
  }
  else if (!value && m_pPSSM)
  {
    DeletePSSM();
  }
}

bool Scene::GetEnableDynamicShadow() const
{
  return m_pContext->GetEnableDynamicShadow();
}
