
#ifndef _RENDER_H
#define _RENDER_H

#include <vector>
#include "../public/macros.h"
#include "../public/core_mem.h"
#include "../visual/i_resource.h"
#include "../visual/i_texture.h"

#include "device_caps.h"

#include "../visual/i_render.h"
#include "render_header.h"

class TRenderAlloc
{
public:
	TRenderAlloc() {}
	~TRenderAlloc() {}

	// 申请内存
	void* Alloc(size_t size) { return CORE_ALLOC(size); }
	// 释放内存
	void Free(void* ptr, size_t size) { CORE_FREE(ptr, size); }
	// 交换
	void Swap(TRenderAlloc& src) {}
};

class TCopyDepthShaderAlloc
{
public:
	TCopyDepthShaderAlloc() {}
	~TCopyDepthShaderAlloc() {}

	// 申请内存
	void* Alloc(size_t size) { return CORE_ALLOC(size); }
	// 释放内存
	void Free(void* ptr, size_t size) { CORE_FREE(ptr, size); }
	// 交换
	void Swap(TCopyDepthShaderAlloc& src) {}
};

enum
{
	DEVICE_STATE_INVALIDE_START,
	DEVICE_STATE_INVALIDE_END,
	DEVICE_STATE_RESTORE,
	DEVICE_STATE_READY,
};

class Render : public IRender
{
public:
	// 深度拷贝的方式
	enum FLAG_DEPTH_SHIFT_ENUM
	{
		ENCODE_RGB888,//直接编码
		ENCODE_RGBA8888,//
		ENCODE_R16,
		ENCODE_R32,
		ENCODE_MAX,
	};
public:
	Render();
	~Render();

	virtual bool Init();
	virtual bool Shut();
	// 是否需要每帧运行（调用ExecFrameBegin和ExecFrameEnd）
	virtual bool NeedExecPerFrame();
	// 每帧开始时调用
	virtual void ExecFrameBegin();
	// 每帧结束时调用
	virtual void ExecFrameEnd();

	// 显示设备尺寸
	void SetDeviceSize(int width, int height);
	int GetDeviceWidth();
	int GetDeviceHeight();
	// 创建颜色渲染目标
	IColorRT* CreateColorRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	// 创建深度渲染目标
	IDepthRT* CreateDepthRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	// 创建立方体贴图
	ITexture* CreateCubeTexture(const char* name, bool async);
	// 创建不可采样的DepthTex
	IDepthTex* CreateDepthTex(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	// 创建可以采样的深度模板贴图
	IDepthStencilRT* CreateDepthStencilRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	// 创建不可以采样的模板图
	IStencilTex* CreateStencilTex(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	DeviceCaps* GetDeviceCaps();
	// 获取设备信息
	PERSISTID GetDeviceCapsID() const;
private:
	PERSISTID   m_DeviceCapsID;
};


#endif