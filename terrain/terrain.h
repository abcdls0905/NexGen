

#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "../public/core_mem.h"
#include "../public/core_type.h"
#include "../utils/array_pod.h"
#include "../visual/i_resource.h"
#include "../visual/i_terrain.h"
#include "../visual/i_render.h"

#include "../algorithm/fractal.h"

class IModel;

class Terrain: public ITerrain
{
protected:
  //IRenderContext* m_pContext;
  bool m_bCastShadow;
  bool m_bReceiveShadow;
  PERSISTID m_SkyID;
  IRender* m_pRender;
  Fractal* m_pFractal;
  TArrayPod<IVisBase*, 1> m_Visuals;

  IModel* m_pFractalTerrain;
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
  // 天空对象
  virtual void SetSkyID(const PERSISTID& value);
  virtual PERSISTID GetSkyID() const;
  virtual void RealizeShadowMap();

  // 需要投射影子
  virtual void SetCastShadow(bool value) { m_bCastShadow = value; }
  virtual bool GetCastShadow() const { return m_bCastShadow; }

  // 需要接受投射影子
  virtual void SetReceiveShadow(bool value)  {m_bReceiveShadow = value; }
  virtual bool GetReceiveShadow() const { return m_bReceiveShadow; }
  virtual void InitFractalTerrain(int w);
  virtual void GenerateFractalTerrain();
};

#endif