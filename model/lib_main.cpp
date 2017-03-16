
#include "../public/module.h"

IIntCreator& ModelSystem_IntCreator();

#define REG_ENTITY(p) IEntCreator& p##_EntCreator();LinkEntCreator(p##_EntCreator());
void lib_main_model()
{
  LinkIntCreator(ModelSystem_IntCreator());
  REG_ENTITY(CResManagerUpdate);
}