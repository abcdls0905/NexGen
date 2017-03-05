
#ifndef _WORLD_H_
#define _WORLD_H_


#include "../public/module.h"
#include "../public/fast_str.h"
#include "../public/core_mem.h"
#include "../public/core_type.h"

#include "../visual/i_world.h"

class IRender;
class World : public IWorld
{
private:
	int m_nWidth;
	int m_nHeight;
	IRender* m_pRender;
	unsigned int m_nBackColor;
public:
	World();
	virtual ~World();

	virtual bool Init(const IVarList& args);
	virtual bool Shut();
	// ÿ֡����
	virtual void Execute(float seconds);

	// ��Ⱦ���ڿ��
	virtual void SetWidth(int value);
	virtual int GetWidth() const;

	// ��Ⱦ���ڸ߶�
	virtual void SetHeight(int value);
	virtual int GetHeight() const;
	// ������Ⱦ�豸
	virtual bool CreateDevice(bool multithreading = false);
	virtual void Display(float offset_seconds);
	// ������ɫ
	virtual void SetBackColor(const char* value);
};

#endif