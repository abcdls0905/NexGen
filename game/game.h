
//��Ϸ���ģ��
#ifndef _GAME_H_
#define _GAME_H_

#include "../public/i_entity.h"

class Game : public IEntity
{
public:
	~Game();
	bool Init(const IVarList& args);
	bool Shut();
	// ������Ϣ����
	bool MsgProc(unsigned int msg, size_t param1, size_t param2,
		int& result);
};

#endif