
#ifndef _MODEL_PLAYER_H_
#define _MODEL_PLAYER_H_

#include <assert.h>
#include "../public/macros.h"
#include "../public/fast_str.h"
#include "../public/core_mem.h"
#include "../public/core_type.h"

#include "../utils/array_pod.h"
#include "model_data.h"
#include "../visual/i_context.h"
#include "model_vf.h"
#include "../visual/i_render.h"
#include "../utils/pod_hash_map.h" 

enum
{
	c_mtxWorld,
	c_mtxWVP,
	c_fBumpScale,
	c_fReflectScale,
	c_fSaturationScale,
	c_fGlowAlpha,
	c_fBlendWeight,
	c_fAlphaRefValue,
	c_fTime,
	c_fGlowSize,
	c_BoneList,
	c_mtxDiffuseTM,
	c_MaterialEmissive,
	c_MaterialSpecular,
	c_fSpecularPower,
	c_MaterialDiffuse,
	c_AppendColor,
	c_MaterialAmbient,
	c_MaterialAmbientEx,
	c_vLightDir,
	c_mtxViewInverse,
	c_LightAmbient,
	c_vViewPos,
	c_vReferPos,
	c_ClipPlane,
	c_LightDiffuse,
	c_FogColor,
	c_FogParam,
	c_FogExpParam,
	c_FallOffParam,
	c_SubSurfaceParam,
	c_LightMapParam,
	c_vLightMapColorScale,
	c_PointLightDiffuse,
	c_PointLightPosition,
	c_FilterParam,
	c_FilterMatrix,
	c_mtxShadowViewProj,
	c_HeightFogColor,
	c_HeightFogParam,
	tex_Diffuse,
	tex_Bump,
	tex_SpecularMap,
	tex_EmissiveMap,
	tex_ReflectionMap,
	tex_LightMap,
	tex_FilterMap,
	tex_KeepoutMap,
	tex_Shadow,
	c_max,
};

struct ShaderKey
{
	void * vs;
	void* ps;
};


template<>
class TPodTraits<ShaderKey>
{
public:
	static size_t GetHash(const ShaderKey& value)
	{
		return size_t(value.ps) + size_t(value.vs);
	}

	static bool Equal(const ShaderKey& s1, const ShaderKey& s2)
	{
		return s1.ps == s2.ps && s1.vs == s2.vs;
	}
};

extern IRender* g_pRender;
class TModelAlloc
{
public:
	TModelAlloc() {}
	~TModelAlloc() {}

	// 申请内存
	void* Alloc(size_t size) { return CORE_ALLOC(size); }
	// 释放内存
	void Free(void* ptr, size_t size) { CORE_FREE(ptr, size); }
	// 交换
	void Swap(TModelAlloc& src) {}
};

class ShaderManager
{
	struct Item
	{
		IShaderProgram* pShader;
		int* pConstValues;
		int MaxConstValue;
	};

	TPodHashMap<ShaderKey, Item, TPodTraits<ShaderKey>, TModelAlloc> m_ShaderPrograms;
	static ShaderManager* s_inst;
	Item m_current;
	IShaderParamOp* m_currentShader;
public:
	ShaderManager()
	{
		m_current.pShader = NULL;
		m_current.pConstValues = NULL;
		m_current.MaxConstValue = 0;
	}

	void Destroy()
	{
		if(s_inst == 0)
			return;
		for(TPodHashMap<ShaderKey, Item, TPodTraits<ShaderKey>, TModelAlloc> ::iterator it = m_ShaderPrograms.Begin(); it!= m_ShaderPrograms.End(); it++)
		{
			if(it.GetData().pConstValues)
			{
				CORE_FREE(it.GetData().pConstValues, it.GetData().MaxConstValue*sizeof(int));
			}
		}
		if(s_inst)
		{
			delete(s_inst);
			s_inst = 0;
		}
	}

	IShaderProgram* GetShader(size_t nVS,size_t nPS,IVertexShader* pVS, IPixelShader* pPS, node_material_t* pMat, model_node_t* pNode,  const char** const_value_list = 0, int const_value_count = 0)
	{
		//这里改进提高查询速度
		ShaderKey key;
		key.vs = pVS;
		key.ps = pPS;

		if(m_ShaderPrograms.Exists(key))
		{
			m_current = m_ShaderPrograms.Find(key).GetData();
			m_currentShader = m_current.pShader->GetParamOp();
			return m_current.pShader;
		}


		const char* ptr[ModelVF::MAX];
		int len;
		pMat->vf.GetNames(ptr, len);

		return GetShader(nVS,nPS,pVS, pPS, ptr, len, key, const_value_list, const_value_count);
	}

	IShaderProgram* GetShader(size_t nVS,size_t nPS,IVertexShader* pVS, IPixelShader* pPS, const char** name, int len, ShaderKey& key, const char** const_value_list = 0, int const_value_count = 0)
	{
		if(key.ps  == 0 && key.vs == 0)
		{
			key.ps = pPS;
			key.vs = pVS;
		}

		if(m_ShaderPrograms.Exists(key))
		{
			m_current = m_ShaderPrograms.Find(key).GetData();
			m_currentShader = m_current.pShader->GetParamOp();
			return m_current.pShader;
		}

		IShaderProgram* pShader = g_pRender->CreateShaderProgram(pVS, pPS, name, len);
		Item item;
		item.pShader = pShader;
		item.pConstValues = 0;
		item.MaxConstValue = 0;
		if(const_value_list && const_value_count)
		{
			item.pConstValues = (int*) CORE_ALLOC(const_value_count*sizeof(fm_uint));
			item.MaxConstValue = const_value_count;
			for(int i = 0; i < item.MaxConstValue; i++)
			{
				item.pConstValues[i] = item.pShader->GetParamOp()->FindParamIdByName(const_value_list[i]);
			}
		}
		//TestErr("ShaderManager::GetShader");
		m_ShaderPrograms.Add(key, item);
		m_current = item;
		m_currentShader = m_current.pShader->GetParamOp();
		return pShader;
	}

	fm_uint GetConstValueId(int index)
	{
		return m_current.pConstValues[index];
	}

	static ShaderManager& Inst()
	{
		if(s_inst == NULL)
		{
			s_inst = new ShaderManager;
		}
		return *s_inst;
	}

	//给Shader设置变量
	inline void SetShaderValue1f(int index, float value)
	{
		m_currentShader->SetParamValue(m_current.pConstValues[index], value);
	}

	inline void SetShaderValue1i(int index, int value)
	{
		m_currentShader->SetParamValue(m_current.pConstValues[index], value);
	}

	inline void SetShaderValue4fv(int index, int count, float* value)
	{
		m_currentShader->SetParamValue(m_current.pConstValues[index], count, (FmVec4*)value);
	}

	inline void SetShaderValue4f(int index, float v1, float v2, float v3, float v4)
	{
		m_currentShader->SetParamValue(m_current.pConstValues[index], v1, v2, v3, v4);
	}

	inline void SetShaderValue4f(int index, const FmVec4& value)
	{
		SetShaderValue4f(index, value.x, value.y, value.z, value.w);
	}

	inline void SetShaderValue4f(const char* name, const FmVec4& value)
	{
		fm_int id = m_currentShader->FindParamIdByName(name);
		m_currentShader->SetParamValue(id, value.x, value.y, value.z, value.w);
	}

	inline void SetShaderValue3f(int index, float v1, float v2, float v3)
	{
		m_currentShader->SetParamValue(m_current.pConstValues[index], v1, v2, v3);
	}

	inline void SetShaderValue3f(int index, const FmVec3& value)
	{
		SetShaderValue3f(index, value.x, value.y, value.z);
	}

	inline void SetShaderValueMat4(int index, const FmMat4& value)
	{
		m_currentShader->SetParamValue(m_current.pConstValues[index], (FmMat4&)value);
	}

	inline void SetTexture2D(int index, fm_int TexID)
	{
		if(m_current.pConstValues[index] < 0)
		{
			return;
		}
		m_currentShader->SetTexture2D(m_current.pConstValues[index], TexID);
	}

	inline void SetTexture3D(int index, fm_int TexID)
	{
		if(m_current.pConstValues[index] < 0)
		{
			return;
		}
		m_currentShader->SetTexture3D(m_current.pConstValues[index], TexID);
	}

	inline void SetTextureCube(int index, fm_int TexID)
	{
		if(m_current.pConstValues[index] < 0)
		{
			return;
		}
		m_currentShader->SetTextureCube(m_current.pConstValues[index], TexID);
	}
};

class TModelPlayerAlloc
{
public:
	TModelPlayerAlloc() {}
	~TModelPlayerAlloc() {}

	// 申请内存
	void* Alloc(size_t size) { return CORE_ALLOC(size); }
	// 释放内存
	void Free(void* ptr, size_t size) { CORE_FREE(ptr, size); }
	// 交换
	void Swap(TModelPlayerAlloc& src) {}
};

class ShaderUserConstValue
{
	struct Value4f
	{
		char name[256];
		int id;
		FmVec4 value;
	};

	TArrayPod<Value4f, 1> m_values4f;
public:
	inline void SetValue(const char* name, const FmVec4& value)
	{
		for(size_t i = 0; i < m_values4f.size(); i++)
		{
			if(strcmp(m_values4f[i].name, name) == 0)
			{
				m_values4f[i].value = value;
				return;
			}
		}
		Value4f v;
		strcpy(v.name, name);
		v.value = value;
		m_values4f.push_back(v);
	}

	inline void UploadGPU()
	{
		for(size_t i = 0; i < m_values4f.size(); i++)
		{
			ShaderManager::Inst().SetShaderValue4f(m_values4f[i].name, m_values4f[i].value);
		}
	}
};


class ModelSystem;
class IRender;
class ITexture;
class IIniFile;
class CResModel;
class CMaterialLoader;

class CModelPlayer;
struct MatInfo
{
public:
  MatInfo()
    :pMat(0)
    ,pNode(0)
    ,bone_matrices(0)
    ,pThis(0)
    ,nColor(0xffffffff)
  {}

  node_material_t* pMat;
  model_node_t* pNode; 
  const FmVec4* bone_matrices;
  CModelPlayer* pThis;
  FmMat4 mtxCurrentTM;
  camera_t camera;
  FmVec3 refer_pos;
  material_info_t* pMatInfo;
  unsigned int nColor;
};

class MatInfoPool
{
  TArrayPod<MatInfo*, 1, TModelPlayerAlloc> m_items;
  int m_index;
public:
  MatInfoPool()
    :m_index(0)
  {}

  ~MatInfoPool()
  {
    for(size_t i = 0; i < m_items.size(); i++)
    {
      delete(m_items[i]);
    }
  }

  MatInfo* Alloc()
  {
    if(m_index < (int)m_items.size())
    {
      return m_items[m_index++];
    }

    MatInfo * p = new MatInfo;
    m_items.push_back(p);
    m_index++;
    return p;
  }

  void RemoveOne()
  {
    Assert(m_index >0);
    m_index --;
  }
};

class CModelPlayer: public IModelPlayer
{
public:
  enum
  {
    //RES_CATEGORY_LIGHT,			// 光照数据
    RES_CATEGORY_MATERIAL,		// 材质数据
  };

  enum MODEL_STATE_ENUM
  {
    MODEL_STATE_INVALID = 0x00000000,
    MODEL_STATE_PLAYING = 0x00000001,
    MODEL_STATE_STOPPED = 0x00000002,
    MODEL_STATE_PAUSE = 0x00000003,
  };
public:
  struct atlas_light_map_t
  {
    size_t nSize;
    float fScaleU;
    float fScaleV;
    float fOffsetU;
    float fOffsetV;
    char strName[1];
  };

private:
  MatInfo* m_MatList;
  IRender* m_pRender;
  IRenderContext* m_pContext;
  CResModel* m_pResModel;
  core_string m_strName;
  double m_dBeginTime;
  double m_dCurrentTime;
  double m_dLastTime;
  FmVec3 m_vScale;
  FmVec3 m_vBoxSize;
  FmVec3 m_Vec3DefaultValve;
  TArrayPod<bool, 1, TModelPlayerAlloc> m_UpdateFlags;
  TArrayPod<ITexture*, 1, TModelPlayerAlloc> m_LightTexs;
  TArrayPod<atlas_light_map_t*, 1, TModelPlayerAlloc> m_AtlasLightMaps;
  TArrayPod<FmVec3, 1, TModelPlayerAlloc> m_LightMapConfig;
  core_string m_strLightFile;
  core_string m_strMaterialFile;
  CMaterialLoader* m_pMaterialLoader;
  unsigned int m_nCustomMaterialNum;
  custom_material_t* m_pCustomMaterials;
  material_info_t* m_pCurMatInfo;
  unsigned int m_nCurrentFrame;
  float m_fCurrentFrameOffset;
  float m_fSpeed;
  unsigned int m_nRefId;
  unsigned int m_nColor;
  int m_nState;
  int m_nSelectHelper;
  int m_nWriteVelocity;
  bool m_bAsyncLoad;
  bool m_bShowBoundBox;
  bool m_bSkeletonMapped;
  bool m_bUseVertexColor;
  bool m_bUseLightMap;
  bool m_bLoop;
  bool m_bSelected;
  bool m_bTraceMainModel;
  bool m_bHide;
  bool m_bLive;
  bool m_bShowHelper;
  bool m_bUnderWater;
  bool m_bFallOff;
  bool m_bPointLighting;
  bool m_bNeedDraw;
  bool m_bKeepOut;
  bool m_bCastShadow;
  bool m_bReceiveShadow;
  FmMat4 m_mtxWorldTM;
  FmMat4 m_mtxPrevWorldTM;
  FmMat4 m_mtxCurrentTM;
  FmMat4* m_mtxNodeCurrentTM;
  cache_skin_t* m_pCacheSkins;
  cache_bone_t* m_pCacheBones;
  core_string m_KeepOutName;
  ITexture* m_KeepOutTexture;
public:
  MatInfoPool m_MatInfoPools;

public:
  CModelPlayer(IRender* pRender, CResModel* pResModel);
  virtual ~CModelPlayer();
private:
  CModelPlayer();
  CModelPlayer(const CModelPlayer&);
  CModelPlayer& operator=(const CModelPlayer&);
public:
  static CModelPlayer* NewInstance(IRender* pRender, 
    CResModel* pResModel);
  // 播放起始时间
  void SetBeginTime(double value) { m_dBeginTime = value; }
  double GetBeginTime() const { return m_dBeginTime; }
  // 创建引用
  bool Create();
  // 结束异步创建 
  bool EndCreate(int category, bool succeed);
  // 释放
  virtual void Release();
  // 是否就绪
  virtual bool IsReady();
  // 材质信息
  virtual size_t GetMaterialCount();
};

#endif