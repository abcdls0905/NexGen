
#include "../public/module.h"
#include "../visual/i_context.h"
#include "../visual/i_render.h"
#include "../public/core_log.h"
#include "global.h"
#include "game.h"

DECLARE_ENTITY(Game, 0, IEntity);

Game::~Game()
{

}

bool Game::Init(const IVarList& args)
{
	g_core = GetCore();
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
