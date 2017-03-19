

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
public:
  Terrain();
  virtual ~Terrain();

  virtual bool Init(const IVarList& args);
  virtual bool Shut();

  // 设置场景上下文
  virtual void SetContext(IRenderContext* value);
  virtual IRenderContext* GetContext() const;
};

#endif