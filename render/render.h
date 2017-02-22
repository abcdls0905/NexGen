
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

	// �����ڴ�
	void* Alloc(size_t size) { return CORE_ALLOC(size); }
	// �ͷ��ڴ�
	void Free(void* ptr, size_t size) { CORE_FREE(ptr, size); }
	// ����
	void Swap(TRenderAlloc& src) {}
};

class TCopyDepthShaderAlloc
{
public:
	TCopyDepthShaderAlloc() {}
	~TCopyDepthShaderAlloc() {}

	// �����ڴ�
	void* Alloc(size_t size) { return CORE_ALLOC(size); }
	// �ͷ��ڴ�
	void Free(void* ptr, size_t size) { CORE_FREE(ptr, size); }
	// ����
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
	// ��ȿ����ķ�ʽ
	enum FLAG_DEPTH_SHIFT_ENUM
	{
		ENCODE_RGB888,//ֱ�ӱ���
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
	// �Ƿ���Ҫÿ֡���У�����ExecFrameBegin��ExecFrameEnd��
	virtual bool NeedExecPerFrame();
	// ÿ֡��ʼʱ����
	virtual void ExecFrameBegin();
	// ÿ֡����ʱ����
	virtual void ExecFrameEnd();

	// ��ʾ�豸�ߴ�
	void SetDeviceSize(int width, int height);
	int GetDeviceWidth();
	int GetDeviceHeight();
	// ������ɫ��ȾĿ��
	IColorRT* CreateColorRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	// ���������ȾĿ��
	IDepthRT* CreateDepthRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	// ������������ͼ
	ITexture* CreateCubeTexture(const char* name, bool async);
	// �������ɲ�����DepthTex
	IDepthTex* CreateDepthTex(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	// �������Բ��������ģ����ͼ
	IDepthStencilRT* CreateDepthStencilRT(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	// ���������Բ�����ģ��ͼ
	IStencilTex* CreateStencilTex(unsigned int width, unsigned int height, TEX_FORMAT format, int type);
	DeviceCaps* GetDeviceCaps();
	// ��ȡ�豸��Ϣ
	PERSISTID GetDeviceCapsID() const;
private:
	PERSISTID   m_DeviceCapsID;
};


#endif