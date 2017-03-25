
#include "../public/module.h"

#define REG_ENTITY(class_name) 	extern IEntCreator& class_name##_EntCreator();\
	LinkEntCreator(class_name##_EntCreator())

void lib_main_world()
{
  REG_ENTITY(World);
  REG_ENTITY(Actor);
  REG_ENTITY(Scene);
  REG_ENTITY(Camera);
}
