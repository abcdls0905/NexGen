
#ifndef _ACTOR_H_
#define _ACTOR_H_


#include "../public/core_mem.h"
#include "../public/fast_str.h"
#include "../public/core_type.h"
#include "../utils/array_pod.h"
#include "../visual/i_actor.h"
#include "../visual/i_actor_callee.h"
#include "../visual/i_model_system.h"
#include "../visual/i_model_player.h"
#include "../visual/i_render.h"

class IIniFile;

class Actor : public IActor
{
private:
  FmMat4 m_mtxWorld;
  IRender* m_pRender;
  core_string m_ConfigFile;
  IModelSystem* m_pModelSystem;
  IModelPlayer* m_pModelPlayer;
  bool m_bVisible;
public:
  Actor();
  virtual ~Actor();
public:
  virtual bool Init(const IVarList& args);
  virtual bool Shut();
  virtual void Load(const char* modelfile, const char* type);
  virtual void Realize();
  virtual void RealizeNormal();
  virtual void RealizeReflect();
  virtual PERSISTID GetLinkObject(const char* name);
  // λ��
  virtual bool SetPosition(float x, float y, float z);
  virtual FmVec3 GetPosition() const;
  // ����
  virtual bool SetAngle(float x, float y, float z);
  virtual FmVec3 GetAngle() const;
  virtual void RealizeShadowMap();
  void UpdateMatrix(IVisBase* pVisBase);
};

#endif