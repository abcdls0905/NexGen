
#ifndef _VISUAL_I_MODEL_PLAYER_H
#define _VISUAL_I_MODEL_PLAYER_H

#include "i_res_base.h"
#include "fm_define.h"
#include "i_model_system.h"

// 顶点缓冲数据
struct vertex_data_t
{
#ifdef CPU_COMPUTE_BONE
	vertex_data_t()
		:pDynamicVertices(0)
	{}
#endif
	unsigned int nStride; // 单个顶点的数据长度
	size_t nCount; // 顶点总数据
	float* pVertices;
#ifdef CPU_COMPUTE_BONE
	unsigned char* pDynamicVertices;
#endif
};

// 创建顶点数据
inline void create_vertex_data(vertex_data_t* pData, unsigned int stride, 
	size_t count)
{
	pData->pVertices = (float*)CORE_ALLOC(stride * count);
	pData->nStride = stride;
	pData->nCount = count;
}

// 释放顶点数据
inline void release_vertex_data(vertex_data_t* pData)
{
	Assert(pData->pVertices != NULL);
	
	CORE_FREE(pData->pVertices, pData->nStride * pData->nCount);
	pData->pVertices = NULL;
	pData->nStride = 0;
	pData->nCount = 0;
}

// 获得指定索引的顶点数据
inline float* get_vertex_by_index(const vertex_data_t* pData, 
	size_t index)
{
	Assert(index < pData->nCount);

	return pData->pVertices + pData->nStride * index;
}

// 模型播放器接口

class IRenderContext;
class IActionPlayer;
struct skt_pose_t;

class IModelPlayer: public IResBase
{
public:

protected:
	// 为了优化
	FmVec3 m_vCenter;
  float m_fRadius;
public:
  virtual bool Draw(const FmPlane* planes, size_t plane_num) = 0;
  virtual void SetWorldMatrix(const FmMat4& mat) = 0;
  virtual bool DrawShadowMap(const FmPlane* planes, size_t plane_num) = 0;
};

#endif // _VISUAL_I_MODEL_PLAYER_H
