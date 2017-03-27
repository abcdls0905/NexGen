
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
  // ��Ⱦ������
  IRenderContext* GetContext() const { return m_pContext; }
  // ��ó�����ͼ
  ISceneView* GetSceneView() const { return m_pSceneView; }
  // ��ÿɼ�����
  IVisBase* GetVisBase(const PERSISTID& id) const;
  // ��ӿ��Ӷ���
  bool AddObject(const PERSISTID& obj, int priority);
  // ɾ�����Ӷ���
  bool RemoveObject(const PERSISTID& obj);
  // �����ȼ�ɾ������
  bool RemoveObjectByPrior(int priority);
  // �������Ӷ���
  virtual PERSISTID Create(const char* name);
  virtual PERSISTID GetCameraID();
  // ��Ӱʵ�����
  PERSISTID GetPSSM() const;
  PERSISTID CreatePSSM();
  bool DeletePSSM();
  virtual void SetEnableDynamicShadow(bool value);
  virtual bool GetEnableDynamicShadow() const;
};

#endif