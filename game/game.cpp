
#include "../public/module.h"
#include "../visual/i_context.h"
#include "../visual/i_render.h"
#include "../public/core_log.h"
#include "../visual/i_scene.h"
#include "../visual/i_world.h"
#include "../visual/i_terrain.h"
#include "global.h"
#include "game.h"
#include "game_control.h"

DECLARE_ENTITY(Game, 0, IEntity);

Game::~Game()
{

}

bool Game::Init(const IVarList& args)
{
  g_core = GetCore();
  g_render = (IRender*)GetCore()->GetInterface("Render");
  g_world = (IWorld*)GetCore()->LookupEntity("World");
  g_scene = (IScene*)g_core->CreateEntity("Scene");
  //创建地形
  PERSISTID terrain_id = g_scene->Create("Terrain");
  ITerrain* pTerrain = (ITerrain*) g_core->GetEntity(terrain_id);
  g_terrain = pTerrain;
  g_scene->AddObject(terrain_id, 0);
  //设置主场景实体
  g_world->SetMainSceneID(g_scene->GetID());
  g_core->AddMsgProc(this, WM_LBUTTONDOWN);
  g_core->AddMsgProc(this, WM_KEYDOWN);
  g_gamecontrol = (GameControl*)g_core->CreateEntity("GameControl");
	return true;
}

bool Game::Shut()
{
	return true;
}

bool Game::MsgProc(unsigned int msg, size_t param1, size_t param2, int& result)
{
	return true;
}
