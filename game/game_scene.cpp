#include "game_scene.h"
#include "helper.h"
#include "../visual/i_world.h"
#include "../visual/i_sky_box.h"
#include "global.h"
#include "../visual/i_context.h"
#include "../define/priority_define.h"
#include "game_util.h"

#define PERFORMANCE_DEBUG

inline const char* gettexpath(const char* file, char* pre = "map/", const char* ext = "dds", char* buf = NULL)
{
  static char buffer[4096];
  if(!buf)
    buf = buffer;
  sprintf(buf, "%s/%s", pre, file);
  return buf;
}

bool SceneManager::Load(const char* pstrfile,IScene* pScene, bool can_load_skybox /*= true*/)
{
  return load_terrain(pstrfile,pScene, can_load_skybox);
}

void SceneManager::Unload()
{
  if(g_terrain)
  {
    g_scene->RemoveObject(g_terrain->GetID());
    g_core->DeleteEntity(g_terrain->GetID());
    g_terrain = NULL;
  }
}

bool SceneManager::load_terrain(const char* pstrfile,IScene* pScene, bool can_load_skybox /*= true*/)
{
  Unload();
  char mbzTemp[512];
  sprintf(mbzTemp,"%s%s",pstrfile,"terrain.ini");
  //先创建一个场景
  IIniFile* pIni = g_core->CreateIniFile(pstrfile);
  pIni->SetFileName(mbzTemp);
  bool b = pIni->LoadFromFile();
  if(!b)
  {
    pIni->Release();
    return false;
  }

  char res_path[256];
  strcpy(res_path, g_core->GetResourcePath());

  //创建地形
  PERSISTID terrain_id = pScene->Create("Terrain");
  ITerrain* pTerrain = (ITerrain*) g_core->GetEntity(terrain_id);
  g_terrain = pTerrain;
  load_skybox(pIni, pTerrain, pScene);
  pTerrain->SetCastShadow(true);
  pScene->AddObject( pTerrain->GetID() , 30 );
  pScene->SetEnableDynamicShadow(true);
  FmVec4 light_diffuse = FmVec4(2.0f, 2.0f, 2.0f, 1.0f);
  IRenderContext* pContext = pTerrain->GetContext();
  pContext->SetVector4(IRenderContext::V_LIGHT_DIFFUSE, light_diffuse);
  FmVec4 light_dir = FmVec4(0.76, 0.64, 0.12, 0.0);
  pContext->SetVector4(IRenderContext::V_LIGHT_DIRECTION, light_dir);
  pIni->Release();
}

void SceneManager::load_skybox(IIniFile* pIni,ITerrain* pTerrain,IScene* pScene)
{
  //创建一个天空实体
  PERSISTID skybox_id = pScene->Create("SkyBox");
  ISkyBox* pSkyBox = (ISkyBox*) g_core->GetEntity(skybox_id);

#define SetSkyBoxFloat(name,ininame)  CHelper::SetPropertyFloat(pSkyBox,name,pIni->ReadFloat("weather",ininame,CHelper::QueryPropertyFloat(pSkyBox,name)))
#define SetSkyBoxString(name,ininame) CHelper::SetPropertyString(pSkyBox,name,pIni->ReadString("weather",ininame,CHelper::QueryPropertyString(pSkyBox,name).c_str()))
#define SetSkyBoxTexture(name,ininame) CHelper::SetPropertyString(pSkyBox,name,gettexpath(pIni->ReadString("weather",ininame,CHelper::QueryPropertyString(pSkyBox,name).c_str()), "map/", "dds"))

  SetSkyBoxFloat("YawSpeed","SkyYawSpeed");
  SetSkyBoxTexture("UpTex","SkyUpTex");
  SetSkyBoxTexture("SideTex","SkySideTex");

  pSkyBox->Load();

  //设置天空
  pTerrain->SetSkyID( skybox_id );

#undef SetSkyBoxTexture
#undef SetSkyBoxString
#undef SetSkyBoxFloat
}


SceneManager g_game_scene;
bool terrain_load(const char* pstrfile, bool can_load_skybox)
{
  IScene* pScene = (IScene*)g_core->LookupEntity("Scene");
  return g_game_scene.Load(pstrfile, pScene, can_load_skybox);
}