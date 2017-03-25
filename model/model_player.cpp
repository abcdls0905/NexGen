
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


// 获得合适的ALPHA测试参考值
int get_current_alpha_ref(float alpha_ref)
{
  if ((alpha_ref > 0.001F) && (alpha_ref <= 1.0F))
  {
    return int(alpha_ref * 255.0F);
  }

  return g_pRender->GetRenderAlphaRef();
}


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

bool CModelPlayer::Draw(const FmPlane* planes, size_t plane_num)
{
  if (!IsReady())
  {
    return false;
  }
  DrawModel(true, 0);
  return true;
} 


void CModelPlayer::UpdateModel(bool bCull)
{

}

void CModelPlayer::DrawModel(bool bCull, MatInfo* MatList)
{
  model_t* pModel = m_pResModel->GetModelData();
  if (pModel == NULL)
  {
    CORE_TRACE_EX("WARNING: [CModelPlayer::DrawModel] pModel == NULL model name is %s", m_pResModel->GetName());
    return;
  }
  m_mtxCurrentTM = m_mtxWorldTM;
  for (unsigned int i = 0; i < pModel->nRootNodeCount; ++i)
  {
    DrawNode(&pModel->RootNodes[i], bCull, MatList);
  }
}

void CModelPlayer::DrawNode(model_node_t* pNode, bool bCull, MatInfo* matinfo)
{
  Assert(pNode != NULL);
  FmMat4 mtxOldWorld = m_mtxCurrentTM;
  int id;
  for (unsigned int m = 0; m < pNode->nMaterialCount; m++)
  {
    node_material_t* pMat = &pNode->Materials[m];
    m_pCurMatInfo = &pMat->MatInfo;
    // 是否有另外指定的材质参数
    DrawMaterial(pMat, pNode, matinfo);
  }
  //draw child nodes
  for (unsigned int child = 0; child < pNode->nChildNodeCount; child++)
  {
    DrawNode(&pNode->ChildNodes[child], bCull, matinfo);
  }
  m_mtxCurrentTM = mtxOldWorld;
}

void CModelPlayer::DrawMaterial(node_material_t* pMat, model_node_t* pNode, MatInfo* matinfo)
{
  MatInfo& _matinfo = *m_MatInfoPools.Alloc(); //
  //matinfo[pMat->nIndexInModel];
  pMat->vf.Parse(pMat->nMaterialInfo, pMat->nMaterialInfoEx, pNode->nModelInfo, pNode->nType);
  _matinfo.pMat = pMat;
  _matinfo.pNode = pNode;
  _matinfo.pThis = this;
  IRenderContext* pContext = g_pRender->GetContext();
  _matinfo.camera = pContext->GetCamera();
  _matinfo.refer_pos = pContext->GetReferPosition();
  _matinfo.pMatInfo = m_pCurMatInfo;
  _matinfo.nColor = m_nColor;

  Assert(pMat != NULL);
  Assert(pNode != NULL);
  if (!CreateVBIB(pMat, pNode))
  {
    return;
  }
  FmVec4 bone_matrices;
  AddDrawMaterialSolidBatch(pMat, pNode, &bone_matrices, _matinfo);
}

bool CModelPlayer::CreateVBIB(node_material_t* pMat, model_node_t* pNode)
{
  if (pMat->pSingleGPUVB == NULL)
  {
    unsigned int vb_size = pMat->nVertexCount * pMat->SingleVB.nStride;
    pMat->pSingleGPUVB = g_pRender->CreateStaticVB(pMat->SingleVB.pVertices, vb_size);
  }
  if (pMat->pGPUIB == NULL)
  {
    unsigned int ib_size = pMat->nIndicesCount * sizeof(unsigned short);
    pMat->pGPUIB = g_pRender->CreateStaticIB(pMat->indices, ib_size);
  }
  return true;
}

void CModelPlayer::AddDrawMaterialSolidBatch(node_material_t* pMat, model_node_t* pNode, const FmVec4* bone_matrices, MatInfo& info)
{
  info.mtxCurrentTM = this->m_mtxCurrentTM;
  //这里新增Early Z优化
  FmVec3 vecTemp = FmVec3(info.mtxCurrentTM._41,info.mtxCurrentTM._42,info.mtxCurrentTM._43) - info.camera.vPosition;
  float d_len = vecTemp.length();
  FmVec3 vecR = pNode->vBoundMax - pNode->vBoundMin;
  float r_len = vecR.length() / 2.0f;//半径
  g_pRender->GetSceneView()->AddSolidBatch(DrawMaterialSolidBatch, &info, "CModelPlayer::AddDrawMaterialSolidBatch");
}

void CModelPlayer::DrawMaterialSolidBatch(void* pdata)
{
  MatInfo* param = (MatInfo*)pdata;
  param->pThis->m_MatInfoPools.RemoveOne();
  param->pThis->DrawMaterialSolid(param);
}

void CModelPlayer::DrawMaterialSolid(const MatInfo* info, bool onlyzpass /*= false*/, bool onlycolor /*= false*/)
{
  node_material_t* pMat = info->pMat;
  model_node_t* pNode = info->pNode;
  const FmVec4* bone_matrices = info->bone_matrices;
  IRenderDrawOp* pDrawOp = g_pRender->GetRenderDrawOp();
  pDrawOp->SetVB( pMat->pSingleGPUVB->GetBuffer());
  pDrawOp->SetIB( pMat->pGPUIB->GetBuffer());
  if (!CreateVDecl(pMat, pNode, false))
	{
		pDrawOp->SetVB( 0 );
		pDrawOp->SetIB( 0 );
		return;
	}
  material_info_t* pMatInfo = info->pMatInfo;
  //m_pCurMatInfo;
  IRenderContext* pContext = g_pRender->GetContext();

  // 环境反射是否开启
  bool reflect_enable = false;

  // 半球环境光
  bool sphere_ambient = pMatInfo->bSphereAmbient;


  // 是否开启静态光照下的人物点光源照明
  bool dynamic_shadow = false;
  bool point_light = (!dynamic_shadow)
    && (pContext->GetInt(IRenderContext::I_ENABLE_ROLE_LIGHT) != 0)
    && (pNode->nType == FXNODE_TYPE_SKIN);

  // 指数雾效
  bool fog_exp = pContext->GetInt(IRenderContext::I_FOG_EXP) != 0;

  // 线性雾效
  bool fog_linear = pContext->GetInt(IRenderContext::I_FOG_LINEAR) != 0;

  // 高度雾效
  bool height_fog = pContext->GetInt(IRenderContext::I_HEIGHT_FOG) != 0;

  bool camera_light = (pNode->nType == FXNODE_TYPE_SKIN) 
    && (pContext->GetInt(IRenderContext::I_ENABLE_CAMERA_LIGHT) != 0);

  //bool gamma = pContext->GetEnableGammaCorrect();
  bool gamma = false;
  bool use_clip_plane = pContext->GetInt(IRenderContext::I_USE_CLIP_PLANE) != 0;

  bool prelight = false;
  bool fix_shadow = false;

  bool dynamic_lighting = true;

  bool filter = pMat->nMaterialInfoEx & MATERIAL_FILTER_MAP_INFO  && info->pMatInfo->bFilterEnable;

  IShaderProgram* pShader = SelectShader(info, pNode, pMat, dynamic_lighting, dynamic_shadow, fog_linear, 
    fog_exp, height_fog, prelight, reflect_enable, fix_shadow, sphere_ambient, 
    point_light, 255, false, camera_light, false, gamma, filter, use_clip_plane, onlyzpass, onlycolor);

  //glUseProgram(pShader->GetProgram());
  pShader->UsedShader();
  IRenderStateOp* pRenderStateOp = pShader->GetRenderStateOp();
  IRenderDrawOp* pRenderDrawOp = pShader->GetRenderDrawOp();
  // 设置渲染状态
  if (pMatInfo->bDoubleSide)
    pRenderStateOp->EnableCullFace(false);
  else
    pRenderStateOp->EnableCullFace(true);

  SetShaderConstValue(pShader, info);

  if(info->pMatInfo->bBlendQuality)
    //二次渲染高质量混合，这里写死，不使用编辑器里的材质数据
      pRenderStateOp->EnableDepthWrite(true);
  else
    pRenderStateOp->EnableDepthWrite(!pMatInfo->bNoZWrite);

  int ib_count = pMat->nIndicesCount;
  pRenderDrawOp->EnableCullFace(false);
  pRenderDrawOp->DrawIndex(IRenderDrawOp::DRAW_TRIANGLES,ib_count,IRenderDrawOp::VERTEX_INDEX_UNSIGNED_SHORT,&((fm_short*)0)[0]);
  if(pMatInfo->bDoubleSide)
  {
    //pRenderStateOp->EnableCullFace(true);
  }
}

bool CModelPlayer::CreateVDecl(node_material_t* pMat, model_node_t* pNode, bool lod)
{
  IRenderDrawOp* pRenderDrawOp = m_pRender->GetRenderDrawOp();
  //vertex_t* pvertex = (vertex_t*)pMat->SingleVB.pVertices;
  int offset = 0;
  int ani_offset = pMat->nSingleVertexSize;
  unsigned int vertex_define = 0;

  ModelVF& vf = pMat->vf;

  int stride = pMat->SingleVB.nStride;

  if(pNode->nModelInfo & MODEL_POSITION_INFO)
  {
    int POS = vf.GetVF(ModelVF::VERTEX);
    pRenderDrawOp->EnableVertexAttribArray(POS);
    pRenderDrawOp->SetVertexAttribPointer(POS,3,IRenderDrawOp::VERTEX_DATA_FLOAT,stride, (fm_void*)offset);
    offset += sizeof(float)*3;
  }
  //NORAML
  if (pNode->nModelInfo & MODEL_NORMAL_INFO)
  {
    int NORMAL = vf.GetVF(ModelVF::NORMAL);
    pRenderDrawOp->EnableVertexAttribArray(NORMAL);
    pRenderDrawOp->SetVertexAttribPointer(NORMAL,3,IRenderDrawOp::VERTEX_DATA_FLOAT,stride, (fm_void*)offset);
    offset += sizeof(float)*3;
  }
  // Diffuse texture
  if (pMat->nMaterialInfo & (MATERIAL_DIFFUSE_MAP_INFO | MATERIAL_BUMP_MAP_INFO
    | MATERIAL_SPECULAR_MAP_INFO | MATERIAL_SELF_ILLUMINATION_INFO))
  {
    int UV = vf.GetVF(ModelVF::TEXCOORD);
    pRenderDrawOp->EnableVertexAttribArray(UV);
    pRenderDrawOp->SetVertexAttribPointer(UV,2,IRenderDrawOp::VERTEX_DATA_FLOAT,stride, (fm_void*)offset);
    offset += sizeof(float)*2;
  }
  return true;
}

void CModelPlayer::SetShaderConstValue(IShaderProgram* pShader, const MatInfo* info)
{
  IRenderContext* pContext = g_pRender->GetContext();
  const camera_t& camera = info->camera; //pContext->GetCamera();
  const FmVec3& refer_pos = info->refer_pos; //pContext->GetReferPosition();
  node_material_t* pMat = info->pMat;
  model_node_t* pNode = info->pNode;
  material_info_t* pMatInfo = info->pMatInfo;
  model_t* pModel = m_pResModel->GetModelData();
  if (pModel == NULL)
  {
    CORE_TRACE_EX("WARNING: [CModelPlayer::SetShaderConstValue] pModel == NULL model name is %s", m_pResModel->GetName());
    return;
  }
  //CB_WORLD
  FmMat4 mtxWorld = info->mtxCurrentTM;//m_mtxCurrentTM;
  mtxWorld._41 -= (float)refer_pos.x;
  mtxWorld._42 -= (float)refer_pos.y;
  mtxWorld._43 -= (float)refer_pos.z;
  //FmMat4Transpose(&mtxWorld, &mtxWorld);
  ShaderManager::Inst().SetShaderValueMat4(c_mtxWorld, mtxWorld);

  FmMat4 mtxWVP;
  FmMat4Multiply(&mtxWVP, &mtxWorld, &camera.mtxViewProjRelative);
  ShaderManager::Inst().SetShaderValueMat4(c_mtxWVP, mtxWVP);	
  
  if(pMatInfo->bNoLight)
  {
    float light_ambient[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
    float light_diffuse[4] = { 0.0F, 0.0F, 0.0F, 1.0F };

    ShaderManager::Inst().SetShaderValue4f(c_LightAmbient, light_ambient);
    ShaderManager::Inst().SetShaderValue4f(c_LightDiffuse, light_diffuse);
  }
  else
  {
    FmVec4 light_ambient = pContext->GetVector4(IRenderContext::V_LIGHT_AMBIENT);
    FmVec4 light_diffuse = pContext->GetVector4(IRenderContext::V_LIGHT_DIFFUSE);
    light_ambient.w = pContext->GetFloat(IRenderContext::F_REFLECT_FACTOR);
    ShaderManager::Inst().SetShaderValue4f(c_LightAmbient, light_ambient);
    ShaderManager::Inst().SetShaderValue4f(c_LightDiffuse, light_diffuse);
  }

  bool blend_quality = pMatInfo->bBlendQuality;
  float alpha_ref;
  if(pMatInfo->fAlphaRef < 0.001f && blend_quality)
    alpha_ref = 0.9f;
  else
    alpha_ref = get_current_alpha_ref(pMatInfo->fAlphaRef) / 255.0F;

  unsigned int frame_num = pModel->nEndFrame - pModel->nStartFrame + 1;
  float frame_time = (m_fCurrentFrameOffset + (float)m_nCurrentFrame) 
    / (float)(frame_num - 1);

  //CB_BUMP_SCALE
  ShaderManager::Inst().SetShaderValue1f(c_fBumpScale, pMat->MatInfo.fBumpScale);
  ShaderManager::Inst().SetShaderValue1f(c_fReflectScale, pMat->MatInfo.fReflectFactor);
  ShaderManager::Inst().SetShaderValue1f(c_fSaturationScale, pMat->MatInfo.fSaturationScale);
  ShaderManager::Inst().SetShaderValue1f(c_fGlowAlpha, 1.0f);

  ShaderManager::Inst().SetShaderValue1f(c_fBlendWeight, m_fCurrentFrameOffset);
  ShaderManager::Inst().SetShaderValue1f(c_fAlphaRefValue, alpha_ref);
  ShaderManager::Inst().SetShaderValue1f(c_fTime, frame_time);
  ShaderManager::Inst().SetShaderValue1f(c_fGlowSize, pMatInfo->fGlowSize);

  //设置骨骼矩阵
//   const FmVec3 & pos = g_pRender->GetContext()->GetReferPosition();
//   if(info->bone_matrices)
//     ShaderManager::Inst().SetShaderValue4fv(c_BoneList, pMat->nMappedBoneCount * 3, (float*)info->bone_matrices);

  if (pMat->nMaterialInfo & MATERIAL_DIFFUSE_UV_ANI)
  {
//     FmMat4 mtxDiffuseTM;
// 
//     CreateTextureMatrix(pMat, &mtxDiffuseTM);
//     ShaderManager::Inst().SetShaderValueMat4(c_mtxDiffuseTM, mtxDiffuseTM);
  }

  //==========================================================================
  //scene相关
  //==========================================================================
  const FmVec3& camera_pos = camera.vPosition;

  FmVec4 light_dir = pContext->GetVector4(IRenderContext::V_LIGHT_DIRECTION);
  FmMat4 mtxViewInverse;
  FmMat4Inverse(&mtxViewInverse, NULL, &camera.mtxViewRelative);
  	// 摄像机位置
	/*
	FmVec4 view_pos(camera_pos.x - refer_pos.x, 
		camera_pos.y - refer_pos.y, camera_pos.z - refer_pos.z, 1.0F);
		*/
	FmVec4 view_pos(camera_pos.x - refer_pos.x, camera_pos.y - refer_pos.y, camera_pos.z - refer_pos.z, 1.0f);
	FmVec4 clip_plane = pContext->GetVector4(IRenderContext::V_CLIP_PLANE);

  //fog
  FmVec4 fog_color = pContext->GetVector4(IRenderContext::V_FOG_CURRENT);
  float fog_start = pContext->GetFloat(IRenderContext::F_FOG_START);
  float fog_end = pContext->GetFloat(IRenderContext::F_FOG_END);
  float fog_height = pContext->GetFloat(IRenderContext::F_FOG_HEIGHT);
  float fog_density = pContext->GetFloat(IRenderContext::F_FOG_DENSITY);
  FmVec4 fog_param(fog_start, 1.0F / (fog_end - fog_start), fog_height, 0.0F);
  FmVec4 fog_exp_param = pContext->GetVector4(IRenderContext::V_FOG_EXP_CURRENT);	

  float heightfog_start = pContext->GetFloat(IRenderContext::F_HEIGHT_FOG_START);
  float heightfog_end = pContext->GetFloat(IRenderContext::F_HEIGHT_FOG_END);
  FmVec4 height_fog_param = FmVec4(heightfog_start, 1.0f / (heightfog_end - heightfog_start), 0.0f, 0.0f);

  FmVec4 height_fog_color = pContext->GetVector4(IRenderContext::V_HEIGHTFOG_CURRENT);

  ShaderManager::Inst().SetShaderValue3f(c_vLightDir, light_dir.x, light_dir.y, light_dir.z);
  ShaderManager::Inst().SetShaderValueMat4(c_mtxViewInverse, mtxViewInverse);

  //SetShaderValue4f(pShader, "c_LightAmbient", 0.5,0.5,0.5,0.5);
  ShaderManager::Inst().SetShaderValue4f(c_vViewPos, view_pos);
  ShaderManager::Inst().SetShaderValue4f(c_vReferPos, refer_pos.x, refer_pos.y, refer_pos.z, 0.0);
  ShaderManager::Inst().SetShaderValue4f(c_ClipPlane, clip_plane.x, clip_plane.y, clip_plane.z, clip_plane.w);
  ShaderManager::Inst().SetShaderValue4f(c_FogColor, fog_color);
  ShaderManager::Inst().SetShaderValue4f(c_FogParam, fog_param);
  ShaderManager::Inst().SetShaderValue4f(c_FogExpParam, fog_exp_param);
  ShaderManager::Inst().SetShaderValue4f(c_HeightFogParam, height_fog_param);
  ShaderManager::Inst().SetShaderValue4f(c_HeightFogColor, height_fog_color); 

  m_ShaderUserConstValue.UploadGPU();
}
