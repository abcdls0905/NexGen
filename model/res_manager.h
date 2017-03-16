
#ifndef _RES_MANAGER_H
#define _RES_MANAGER_H

#include "../public/macros.h"
#include "../public/persistid.h"
#include "../public/core_mem.h"
#include "../utils/string_pod.h"
#include "../public/i_entity.h"

class TResManagerAlloc
{
public:
	TResManagerAlloc() {}
	~TResManagerAlloc() {}

	// 申请内存
	void* Alloc(size_t size) { return CORE_ALLOC(size); }
	// 释放内存
	void Free(void* ptr, size_t size) { CORE_FREE(ptr, size); }
	// 交换
	void Swap(TResManagerAlloc& src) {}
};

// 资源管理

class CResModel;
class ModelSystem;

class CResManager
{
private:
	typedef TStringPod<char, CResModel*, TStringTraitsCI<char>, 
		TResManagerAlloc> model_container_t;

public:
	CResManager();
	~CResManager();

	// 初始化
	bool Initialize(ModelSystem* pModelSystem);
	// 释放所有贴图
	bool ReleaseAllTextures();

	// 创建静态模型
	CResModel* CreateResModel(const char* name, const char* tex_paths, 
		bool async);
	// 释放静态模型
	bool ReleaseResModel(CResModel* pResModel);
	
	// 导出所有的资源信息
	bool DumpAll(const char* file_name);

	// 初始化（可创建其他附属实体）
	virtual bool Init(const IVarList& args);
	// 关闭（可删除其他引用的附属实体，而资源释放应在析构函数中进行）
	virtual bool Shut();

	// 每帧执行逻辑（如果已添加到运行队列）
	virtual void Execute(float seconds);
private:
	CResManager(const CResManager&);
	CResManager& operator=(const CResManager&);	

private:
	ModelSystem* m_pModelSystem;
	model_container_t m_ResModels;
};

class CResManagerUpdate : public IEntity
{
public:
	CResManager* res_manager;
public:
	virtual bool Init(const IVarList& args);
	virtual bool Shut(){ return true;}
	virtual void Execute(float seconds);
};

#endif // _RES_MANAGER_H
