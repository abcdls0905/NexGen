
#include "../public/module.h"

#define REG_ENTITY(p) IEntCreator& p##_EntCreator();LinkEntCreator(p##_EntCreator());


void lib_main_game()
{
  REG_ENTITY(Game);
  REG_ENTITY(GameControl);
#ifdef _WIN32
  REG_ENTITY(WinFreeCamera);
#endif
  REG_ENTITY(Camera3rd);
  REG_ENTITY(CameraControl);
}