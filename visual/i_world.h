
#ifndef _I_WORLD_H_
#define _I_WORLD_H_

#include "../public/module.h"

class IWorld : public IEntity
{
public:
	// ��Ⱦ���ڿ��
	virtual void SetWidth(int value) = 0;
	virtual int GetWidth() const = 0;

	// ��Ⱦ���ڸ߶�
	virtual void SetHeight(int value) = 0;
	virtual int GetHeight() const = 0;
	// ������Ⱦ�豸
	virtual bool CreateDevice(bool multithreading = false) = 0;
	// ÿ֡��Ⱦ
	virtual void Display(float offset_seconds) = 0;
	// ������ɫ
	virtual void SetBackColor(const char* value) = 0;
};

#endif