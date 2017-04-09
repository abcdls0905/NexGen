
#ifndef _GAME_CONTROL_H_
#define _GAME_CONTROL_H_

#include "../math/fm_math.h"
#include "../public/i_entity.h"
#include "../define/fmtouchdefine.h"

#include <string>
#include <vector>
#include <map>

class GameControl : public IEntity
{
public:
  GameControl(void);
  ~GameControl(void);

  virtual bool Init(const IVarList& args);
  virtual bool Shut();
  virtual void Execute(float seconds);
  virtual bool MsgProc(unsigned int msg, size_t param1, size_t param2, int& result);
private:
  bool MsgKeyDown(size_t param1, size_t param2);
};

#endif