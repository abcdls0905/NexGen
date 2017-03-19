

#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "../public/core_mem.h"
#include "../public/core_type.h"
#include "../utils/array_pod.h"
#include "../visual/i_resource.h"
#include "../visual/i_terrain.h"
#include "../visual/i_render.h"

class Terrain: public ITerrain
{
protected:
  //IRenderContext* m_pContext;
  IRender* m_pRender;
  TArrayPod<IVisBase*, 1> m_Visuals;
public:
  Terrain();
  virtual ~Terrain();

  virtual bool Init(const IVarList& args);
  virtual bool Shut();

  // 设置场景上下文
  virtual void SetContext(IRenderContext* value);
  virtual IRenderContext* GetContext() const;
  virtual void Update(float seconds);
  virtual void Realize();
  // 添加人物角色
  bool AddVisualRole(const char* name, const PERSISTID& id);
  // 添加地面物体
  bool AddVisual(const char* name, const PERSISTID& id);
  // 添加地面物体
  virtual int AddVisBase(const char* name, IVisBase* pVisBase, 
    bool is_role, float clip_radius);
};

#endif