
#ifndef _WORLD_H_
#define _WORLD_H_


#include "../public/module.h"
#include "../public/fast_str.h"
#include "../public/core_mem.h"
#include "../public/core_type.h"

#include "../visual/i_world.h"
#include "Scene.h"

class IRender;
class Scene;
class Gui;

class World : public IWorld
{
private:
	int m_nWidth;
	int m_nHeight;
	IRender* m_pRender;
  unsigned int m_nBackColor;
  Scene* m_pMainScene;
  Gui* m_pMainGui;
public:
	World();
	virtual ~World();

	virtual bool Init(const IVarList& args);
	virtual bool Shut();
	// 每帧更新
	virtual void Execute(float seconds);

	// 渲染窗口宽度
	virtual void SetWidth(int value);
	virtual int GetWidth() const;

	// 渲染窗口高度
	virtual void SetHeight(int value);
	virtual int GetHeight() const;
	// 创建渲染设备
	virtual bool CreateDevice(bool multithreading = false);
	virtual void Display(float offset_seconds);
	// 背景颜色
  virtual void SetBackColor(const char* value);

  // 主场景对象
  void SetMainSceneID(const PERSISTID& id);
  PERSISTID GetMainSceneID() const;
  IScene* GetMainScene() const { return m_pMainScene; }
  // 主界面对象
  void SetMainGuiID(const PERSISTID& id);
};

#endif