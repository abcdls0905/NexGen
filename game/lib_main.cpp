
#include "../public/module.h"
#include "game.h"

#define REG_ENTITY(p) IEntCreator& p##_EntCreator();LinkEntCreator(p##_EntCreator());


void lib_main_game()
{
	REG_ENTITY(Game);
}