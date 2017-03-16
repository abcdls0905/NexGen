
#include "model_player.h"

#include "../public/macros.h"
#include "../public/fast_str.h"
#include "../public/core_mem.h"
#include "../public/core_type.h"

#include "material_loader.h"
//#include "../visual/dx_scene_batch.h"
#include "../visual/i_model_player.h"
#include "../visual/i_model_system.h"
#include "../utils/array_pod.h"
#include "model_data.h"
#include "../visual/i_context.h"
#include "model_vf.h"
#include "res_model.h"
#include <assert.h>
#include "../visual/i_render.h"


// 获得引用编号
static unsigned int get_ref_id()
{
  static unsigned int s_ref_id = 0;

  return ++s_ref_id;
}

CModelPlayer* CModelPlayer::NewInstance(IRender* pRender, 
                                        CResModel* pResModel)
{
  CModelPlayer* p = (CModelPlayer*)CORE_ALLOC(sizeof(CModelPlayer));

  new (p) CModelPlayer(pRender, pResModel);

  return p;
}

bool CModelPlayer::Create()
{
  model_t* pModel = m_pResModel->GetModelData();

  if (pModel == NULL)
  {
    CORE_TRACE_EX("WARNING: [CModelPlayer::Create] pModel == NULL model name is %s", m_pResModel->GetName());
    return false;
  }

  double beg_time = m_pRender->GetCore()->GetTotalSeconds();

  m_mtxPrevWorldTM = m_mtxWorldTM;
  m_dLastTime = beg_time;
  m_nState = MODEL_STATE_PLAYING;
  m_nRefId = get_ref_id();
  m_mtxNodeCurrentTM = (FmMat4*)CORE_ALLOC(
    sizeof(FmMat4) * pModel->nNodeCount);
  memset(m_mtxNodeCurrentTM, 0, sizeof(FmMat4) * pModel->nNodeCount);

  m_MatList = new MatInfo[GetMaterialCount()];
  return true;
}

CModelPlayer::CModelPlayer(IRender* pRender, CResModel* pResModel)
{
  Assert(pRender != NULL);
  Assert(pResModel != NULL);

  m_pRender = pRender;

  m_KeepOutTexture = NULL;
  m_bKeepOut = false;
  m_pContext = NULL;
  m_pResModel = pResModel; 
  m_dBeginTime = 0.0;
  m_dCurrentTime = 0.0;
  m_dLastTime = 0.0;
  m_pMaterialLoader = NULL;
  m_nCustomMaterialNum = 0;
  m_pCustomMaterials = NULL;
  m_pCurMatInfo = NULL;
  m_vScale = FmVec3(1.0F, 1.0F, 1.0F);
  m_vBoxSize = FmVec3(0.0F, 0.0F, 0.0F);
  m_vCenter = FmVec3(0.0F, 0.0F, 0.0F);
  m_Vec3DefaultValve = FmVec3(1.0f, 1.0f, 1.0f);
  m_fRadius = 0.0F;
  m_nCurrentFrame = 0;
  m_fCurrentFrameOffset = 0.0F;
  m_fSpeed = 1.0F;
  m_nRefId = 0;
  m_nColor = 0xFFFFFFFF;
  m_nState = 0;
  m_nSelectHelper = -1;
  m_nWriteVelocity = 0;
  m_bAsyncLoad = false;
  m_bShowBoundBox = false;
  m_bSkeletonMapped = false;
  m_bUseVertexColor = false;
  m_bUseLightMap = false;
  m_bLoop = true;
  m_bSelected = false;
  m_bTraceMainModel = false;
  m_bHide = false;
  m_bLive = false;
  m_bShowHelper = false;
  m_bUnderWater = false;
  m_bFallOff = true;
  m_bPointLighting = true;
  m_bNeedDraw = false;
  m_bCastShadow = false;
  m_bReceiveShadow = false;
  FmMatrixIdentity(&m_mtxWorldTM);
  FmMatrixIdentity(&m_mtxPrevWorldTM);
  FmMatrixIdentity(&m_mtxCurrentTM);
  m_mtxNodeCurrentTM = NULL;
  m_pCacheSkins = NULL;
  m_pCacheBones = NULL;
  m_MatList = NULL;
}

CModelPlayer::CModelPlayer()
{

}

CModelPlayer::CModelPlayer(const CModelPlayer&)
{

}

CModelPlayer::~CModelPlayer()
{
  delete[] m_MatList;

  m_pResModel->Release();
  SAFE_RELEASE(m_pMaterialLoader);
}

CModelPlayer& CModelPlayer::operator=(const CModelPlayer&)
{
  return *this;
}

bool CModelPlayer::EndCreate(int category, bool succeed)
{
  if (category == RES_CATEGORY_MATERIAL)
  {
    SAFE_RELEASE(m_pMaterialLoader);
  }
  return true;
}

void CModelPlayer::Release()
{
  if (this->DecRefs() == 0)
  {
    CORE_DELETE(this);
  }
}

bool CModelPlayer::IsReady()
{
  if (!m_pResModel->IsReady())
  {
    return false;
  }
  return true;
}

size_t CModelPlayer::GetMaterialCount()
{
  if (NULL == m_pResModel)
  {
    return 0;
  }
  model_t* pModel = m_pResModel->GetModelData();
  if (pModel == NULL)
  {
    CORE_TRACE_EX("WARNING: [CModelPlayer::GetMaterialCount] pModel == NULL model name is %s", m_pResModel->GetName());
    return 0;
  }
  return pModel->nMaterialCount;
}
