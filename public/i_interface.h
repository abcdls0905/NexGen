
#ifndef _PUBLIC_I_INTERFACE_H
#define _PUBLIC_I_INTERFACE_H

#include "i_core.h"

// 功能接口

class IIntCreator;

class IInterface
{
public:
	IInterface()
	{
		m_pCore = NULL;
		m_pCreator = NULL;
	}

	virtual ~IInterface() = 0;

	// 初始化
	virtual bool Init() = 0;
	// 关闭
	virtual bool Shut() = 0;

	// 是否需要每帧运行（调用ExecFrameBegin和ExecFrameEnd）
	virtual bool NeedExecPerFrame() { return false; }
	// 每帧开始时调用
	virtual void ExecFrameBegin() {}
	// 每帧结束时调用
	virtual void ExecFrameEnd() {}

	// 释放
	virtual void Release()
	{
		m_pCore->ReleaseInterface(this);
	}

	// 获得核心接口
	ICore* GetCore() const
	{
		return m_pCore;
	}

	// 获得创建器
	IIntCreator* GetCreator() const
	{
		return m_pCreator;
	}

private:
	IInterface(const IInterface&);
	IInterface& operator=(const IInterface&);

	void SetCore(ICore* value)
	{
		m_pCore = value;
	}

	void SetCreator(IIntCreator* value)
	{
		m_pCreator = value;
	}

private:
	ICore* m_pCore;
	IIntCreator* m_pCreator;
	//friend class CIntManager;
};

inline IInterface::~IInterface() {}

#endif // _PUBLIC_I_INTERFACE_H
