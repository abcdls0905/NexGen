
#ifndef _SCENE_H_
#define _SCENE_H_

#include "../public/module.h"
#include "../visual/i_resource.h"
#include "../visual/i_scene.h"
#include "camera.h"

class IRender;
class IRenderContext;
class IVisBase;
class ISceneView;
class PSSM;

class Scene : public IScene
{
private:
  struct object_t
  {
    PERSISTID ObjectId;
    int nPrior;
    object_t* pNext;
  };
  SCENETYPE m_scenetype;
private:
  IRender* m_pRender;
  IRenderContext* m_pContext;
  ISceneView* m_pSceneView;
  object_t* m_pObjects;
  Camera* m_pCamera;
  PSSM* m_pPSSM;
public:
  Scene();
  virtual ~Scene();

  virtual bool Init(const IVarList& args);
  virtual bool Shut();

  void Update(float seconds);
  void Realize();
  // 渲染器参数
  IRenderContext* GetContext() const { return m_pContext; }
  // 获得场景视图
  ISceneView* GetSceneView() const { return m_pSceneView; }
  // 获得可见对象
  IVisBase* GetVisBase(const PERSISTID& id) const;
  // 添加可视对象
  bool AddObject(const PERSISTID& obj, int priority);
  // 删除可视对象
  bool RemoveObject(const PERSISTID& obj);
  // 按优先级删除对象
  bool RemoveObjectByPrior(int priority);
  // 创建可视对象
  virtual PERSISTID Create(const char* name);
  virtual PERSISTID GetCameraID();
  // 阴影实体对象
  PERSISTID GetPSSM() const;
  PERSISTID CreatePSSM();
  bool DeletePSSM();
  virtual void SetEnableDynamicShadow(bool value);
  virtual bool GetEnableDynamicShadow() const;
};

#endif