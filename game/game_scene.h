
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <vector>
#include "../public/module.h"
#include "../public/core_type.h"
#include "../visual/i_scene.h"
#include "../visual/i_terrain.h"
#include "../visual/i_terrain.h"
#include "../public/i_ini_file.h"

class SceneManager
{
private:
  std::vector<PERSISTID> m_vecSceneObjEnt;//加到场景里的实体列表
public:
  bool Load(const char* pstrfile,IScene* pScene, bool can_load_skybox = true);
  void Unload();
private:
  //加载地表
  bool load_terrain(const char* pstrfile,IScene* pScene, bool can_load_skybox = true);
  //加载天空盒
  void load_skybox(IIniFile* pIni,ITerrain* pTerrain,IScene* pScene);
};

#endif