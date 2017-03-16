
#include "res_manager.h"
#include "res_model.h"
#include "model_system.h"
#include "../visual/i_render.h"
#include "../visual/i_resource.h"
#include "../public/core_mem.h"
#include "../public/core_file.h"
#include "../public/portable.h"

DECLARE_ENTITY(CResManagerUpdate, 0, IEntity);

// CResManager

CResManager::CResManager()
{
	m_pModelSystem = NULL;
}

CResManager::~CResManager()
{
	for (model_container_t::iterator it3 = m_ResModels.Begin(); 
		it3 != m_ResModels.End(); ++it3)
	{
		CResModel* p = it3.GetData();

		p->Destroy();
	}
}

bool CResManager::Initialize(ModelSystem* pModelSystem)
{
	Assert(pModelSystem != NULL);
	m_pModelSystem = pModelSystem;
	return true;
}

bool CResManager::ReleaseAllTextures()
{
	for (model_container_t::iterator it = m_ResModels.Begin(); 
		it != m_ResModels.End(); ++it)
	{
		CResModel* p = it.GetData();
		p->ReleaseTextures();
	}
	return true;
}

CResModel* CResManager::CreateResModel(const char* name, 
	const char* tex_paths, bool async)
{
	CResModel* p;
	
	if (m_ResModels.GetData(name, p))
	{
		p->IncRefs();
		return p;
	}
	
	CResModel* pResModel = CResModel::NewInstance();
	
	pResModel->SetManager(this);
	pResModel->SetName(name);
	pResModel->SetTexPaths(tex_paths);
	
	if (async && pResModel->CanAsync())
	{
		// 创建异步加载的相关内容
		if (!pResModel->BeginCreate())
		{
			pResModel->Destroy();
			return NULL;
		}
		
		// 请求线程加载
		if (!m_pModelSystem->GetRender()->RequestLoadResource( 
			CResModel::RES_CATEGORY_MODEL, pResModel->GetLoader()))
		{
			pResModel->Destroy();
			return NULL;
		}
		
		// 设置正在加载状态
		pResModel->SetState(CResModel::STATE_LOADING);
	}
	else
	{
		// 同步加载
		if (!pResModel->Create())
		{
			pResModel->Destroy();
			return NULL;
		}
		
		// 设置资源就绪
		pResModel->SetState(CResModel::STATE_READY);
	}
	
	m_ResModels.Add(name, pResModel);
	
	return pResModel;
}

bool CResManager::ReleaseResModel(CResModel* pResModel)
{
	Assert(pResModel != NULL);

    pResModel->DecRefs();

	if (0 == pResModel->GetRefs())
	{
		pResModel->time = 0.0;
	}
	
	return true;
}

bool CResManager::DumpAll(const char* file_name)
{
	Assert(file_name != NULL);

	FILE* fp = core_file::fopen(file_name, "wb");

	if (NULL == fp)
	{
		return false;
	}

	// 模型
	int count = 0;

	for (model_container_t::iterator it1 = m_ResModels.Begin(); 
		it1 != m_ResModels.End(); ++it1)
	{
		CResModel* p = it1.GetData();

		fprintf(fp, "geometry: %s refs: %d state: %d\r\n", 
			p->GetName(), p->GetRefs(), p->GetState());
		++count;
	}

	fprintf(fp, "\r\ntotal %d models\r\n\r\n", count);

	core_file::fprintf(fp, "\r\ntotal %d animates\r\n\r\n", count);

	core_file::fprintf(fp, "\r\ntotal %d action sets\r\n\r\n", count);
	core_file::fclose(fp);

	return true;
}

// 初始化（可创建其他附属实体）
bool CResManager::Init(const IVarList& args)
{
//	g_pCore->AddExecute(this);
	return true;
}

// 关闭（可删除其他引用的附属实体，而资源释放应在析构函数中进行）
bool CResManager::Shut()
{
	return true;
}

#define IDLE_TIME 300.0f

void CResManager::Execute(float seconds)
{
	for (model_container_t::iterator it1 = m_ResModels.Begin(); 
		it1 != m_ResModels.End(); ++it1)
	{
		CResModel* pResModel = it1.GetData();
		
		if((pResModel->GetRefs() < 1) && (pResModel->time += seconds) > IDLE_TIME)
		{
			m_ResModels.Remove(pResModel->GetName());
			pResModel->Destroy();
			break;
		}
	}
}

bool CResManagerUpdate::Init(const IVarList& args)
{
	g_pCore->AddExecute(this);
	return true;
}

void CResManagerUpdate::Execute(float seconds)
{
	res_manager->Execute(seconds);
};