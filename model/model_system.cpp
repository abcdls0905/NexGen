
#include "model_system.h"
#include "res_manager.h"
#include "res_model.h"
#include "model_player.h"
#include "../visual/i_render.h"
#include "../public/core_log.h"
#include "../public/core_mem.h"

DECLARE_FUNCTION_INTERFACE(ModelSystem);

IRender* g_pRender = NULL;
// ���ģ������
bool g_bCheckModelData = false;
// ��ʾģ�ͽڵ�İ�Χ��
bool g_bShowNodeBoundingBox = false;

void shader_initialize();
void shader_release();
// ��ʼ�����ʼ���������
void init_material_key_index();
// ��ʼ���ڵ����������
void init_node_key_index();

ModelSystem::ModelSystem()
{
	m_pRender = NULL;
	m_pResManager = CORE_NEW(CResManager);
	m_pResManager->Initialize(this);
}

ModelSystem::~ModelSystem()
{
	shader_release();
	CORE_DELETE(m_pResManager);
}

bool ModelSystem::Init()
{
	m_pRender = (IRender*)GetCore()->GetInterface("Render");
	CResManagerUpdate* pResManagerUpdate = (CResManagerUpdate*)GetCore()->CreateEntity("CResManagerUpdate");
	pResManagerUpdate->res_manager = m_pResManager;

	if (NULL == m_pRender)
	{
		CORE_TRACE("(ModelSystem::Init)no DxRender");
		return false;
	}

	g_pRender = m_pRender;

	shader_initialize();
	init_node_key_index();
	init_material_key_index();

	return true;
}

bool ModelSystem::Shut()
{
	if (m_pRender)
	{
		// �ͷ�������ͼ�Ա�֤�����DxRender������
		m_pResManager->ReleaseAllTextures();
	}

	SAFE_RELEASE(m_pRender);
	
	return true;
}

void ModelSystem::SetCheckModelData(bool value)
{
	g_bCheckModelData = value;
}

bool ModelSystem::GetCheckModelData()
{
	return g_bCheckModelData;
}

void ModelSystem::SetShowNodeBoundingBox(bool value)
{
	g_bShowNodeBoundingBox = value;
}

bool ModelSystem::GetShowNodeBoundingBox()
{
	return g_bShowNodeBoundingBox;
}

void ModelSystem::DrawBoundingBox(const FmVec3* min, 
	const FmVec3* max, unsigned int cr_start, unsigned int cr_end, 
	float scale)
{
}

void ModelSystem::DrawPoint(const FmVec3* position, unsigned int cr, 
	float size, bool balance)
{
}

IModelPlayer* ModelSystem::CreateModelPlayer(const char* name, 
	double beg_time, const char* tex_paths, bool async)
{
	CResModel* pResModel = m_pResManager->CreateResModel(name, tex_paths, 
		async);

	if (NULL == pResModel)
	{
		return NULL;
	}

	CModelPlayer* pModelPlayer = CModelPlayer::NewInstance(m_pRender, 
		pResModel);

	pModelPlayer->SetName(name);
	pModelPlayer->SetBeginTime(beg_time);

	if (pResModel->IsLoadComplete())
	{
		pModelPlayer->Create();
	}
	
	return pModelPlayer;
}

IActionPlayer* ModelSystem::CreateActionPlayer(const char* name, 
	const char* append_path, const char* def_action, void* trigger_proc, 
	void* user_pointer, bool async)
{
  return 0;
}

// ������̬ģ��
CResModel* ModelSystem::CreateResModel(const char* name, const char* tex_paths, bool async)
{
    return m_pResManager->CreateResModel(name, tex_paths, async);
}

// �ͷž�̬ģ��
void ModelSystem::ReleaseResModel(CResModel* pResModel)
{
    m_pResManager->ReleaseResModel(pResModel);
}

// ������������
CSkeleton* ModelSystem::CreateSkeleton(const char* name, bool async)
{
  return 0;
}

// �ͷŹ�������
void ModelSystem::ReleaseSkeleton(CSkeleton* pSkeleton)
{
}

// ����������
CActionSet* ModelSystem::CreateActionSet(const char* name, const char* append_path, bool async)
{
  return 0;
}

// �ͷŶ�����
void ModelSystem::ReleaseActionSet(CActionSet* pActionSet)
{
}

bool ModelSystem::DumpResource(const char* file_name)
{
	return m_pResManager->DumpAll(file_name);
}

int ModelSystem::CollectUnuseAction(int seconds)
{
  return 0;
}
