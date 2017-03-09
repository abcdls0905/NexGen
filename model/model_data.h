
#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_

#include "../visual/i_resource.h"
#include "../visual/i_texture.h"
#include "../public/core_log.h"
#include "../visual/i_model_player.h"
#include "location.h"

const unsigned int MODEL_POSITION_POS = 0;
const unsigned int MODEL_NORMAL_POS = 1;
const unsigned int MODEL_COLOR_POS = 2;
const unsigned int MODEL_ILLUM_POS = 3;
const unsigned int MODEL_TANGENT_POS = 4;
const unsigned int MODEL_BINORMAL_POS = 5;
const unsigned int MODEL_MATRIX_POS = 6;

const unsigned int MATERIAL_AMBIENT_POS = 0;
const unsigned int MATERIAL_DIFFUSE_POS = 1;
const unsigned int MATERIAL_SPECULAR_POS = 2;
const unsigned int MATERIAL_EMISSIVE_POS = 3;
const unsigned int MATERIAL_OPACITY_POS = 4;
const unsigned int MATERIAL_GLOSS_POS = 5;
const unsigned int MATERIAL_DIFFUSE_MAP_POS = 7;
const unsigned int MATERIAL_BUMP_MAP_POS = 8;
const unsigned int MATERIAL_SPECULAR_MAP_POS = 9;
const unsigned int MATERIAL_SPECULAR_LEVEL_MAP_POS = 10;
const unsigned int MATERIAL_GLOSSINESS_MAP_POS = 11;
const unsigned int MATERIAL_REFLECTION_ENV_MAP_POS = 12;
const unsigned int MATERIAL_LIGHTMAP_POS = 13;

const unsigned int MAX_VERTEX_BONE_BLEND = 4;
const unsigned int MAX_BONE = 78;


// 贴图数据
struct texture_info_t
{
	char* pFileName; // 贴图文件名 
	unsigned int nFileNameLen;
	ITexture* pTex; // 贴图资源指针
};

struct filter_param
{
	float fScale;
	float fBias;
	float fScaleAlpha;
	float fBiasAlpha;
};

// 材质数据
struct material_info_t
{
	FmVec3	vAmbient;	// 环境光
	FmVec3	vAmbientEx; //环境光扩展
	FmVec3 vDiffuse;	// 漫反射
	FmVec3 vSpecular;	// 高光反射
	FmVec3 vEmissive;	// 自发光	

	float fGlossiness;		// 光泽度
	float fOpacity;			// 透明度
	float fAlphaRef;		// ALPHA测试参考值
	float fSubSurfaceScale;	// 次表面散射范围
	float fSubSurfaceFactor;// 次表面强度因子
	float fReflectFactor;	// 环境反射强度
	float fBumpScale;		// 法线强度
	float fSaturationScale;	// 材质饱和度
	float fHeightScale;		// 表面细分高度

	filter_param filterParam; // 遮罩参数

	// 材质效果开关
	bool bSpecularEnable;	// 高光开启
	bool bEmissiveEnable;	// 自发光开启
	bool bOpacityEnable;	// 透明开启
	bool bReflectEnable;	// 使用反射贴图
	bool bBumpMapEnable;	// 使用法线贴图
	bool bTessellationEnable; // 使用表面细分
	bool bFilterEnable;		// 使用遮罩贴图

	// 渲染状态
	bool bAlphaTest;		// ALPHA测试
	bool bAlphaTestGreaterQual;	//ALPHA测试函数是否为GREATERQUAL
	bool bBlend;			// ALPHA混合
	bool bBlendEnhance;		// 高亮ALPHA混合
	bool bBlendQuality;		// 二次渲染高质量混合
	bool bNoZWrite;			// 不写入深度缓冲
	bool bNoZTest;			// 无深度测试
	bool bDoubleSide;		// 双面显示
	bool bVolumeTexture;	// 体积纹理
	bool bBlendDownLevel;	// 半透明物体降级显示

	// 采样方式
	bool bSamplerClamp;		// 采样坐标过界后停留在边界上

	// 特殊效果显示
	bool bUIFloat;			// 属于界面的物体
	bool bNoLight;			// 无需动态光照
	bool bSphereAmbient;	// 简易半球光照	
	bool bDisappear;		// 淡入淡出（透明必须开启）
	bool bSkinEffect;		// 皮肤效果
	bool bHairEffect;		// 毛发效果

	// 泛光效果
	bool bGlowEnable;		// 泛光开关
	bool bGlowEntire;		// 是否整体泛光
	bool bGlowEmissive;		// 自发光颜色泛光
	bool bGlowEmissiveMap;	// 自发光贴图泛光
	bool bGlowDiffuse;		// 漫反射泛光
	bool bGlowVertexColor;	// 顶点色泛光
	bool bGlowAppendColor;	// 附加色泛光
	bool bGlowEnhance;		// 高亮泛光
	bool bGlowBlur;			// 泛光模糊
	float fGlowSize;		// 泛光大小
	float fGlowAlpha;		// 泛光透明度

	// 轮廓光（使用自发光颜色）
	bool bRimLight;			// 轮廓光开关
	FmVec3 vRimColor;		// 轮廓光颜色

	float fRimIntensity;	// 轮廓光亮度
	float fRimPower;		// 轮廓光指数

	// 专门用途的模型
	bool bRefraction;		// 屏幕扭曲（折射）
	bool bApplique;			// 模板贴花
	bool bSceneFog;			// 场景体积雾
	bool bForbidStandModel;	// 禁止站立碰撞辅助模型（不可见）
	bool bPhaseMonitor;		// 使用相位监视
	//unsigned int nShaderInfo;
	texture_info_t DiffuseMap;			// 漫反射贴图
	texture_info_t BumpMap;				// 法线贴图
	texture_info_t LightMap;			// 光贴图
	texture_info_t SpecularMap;			// 高光贴图
	texture_info_t SpecularLevelMap;	// 高光亮度贴图	
	texture_info_t GlossinessMap;		// 光泽度贴图
	texture_info_t EmissiveMap;			// 自发光贴图
	texture_info_t ReflectionMap;		// 反射贴图
	texture_info_t DisplacementMap;		// 位移贴图
	texture_info_t FilterMap;			// 遮罩贴图
};


// 节点材质
struct node_material_t
{
	unsigned int nVertexCount;
	vertex_data_t SingleVB;				//顶点信息
	IStaticIB* pGPUIB;					//顶点缓冲
	IStaticVB* pSingleGPUVB;
	material_info_t MatInfo; // 材质数据
};

// 模型节点
struct model_node_t
{
	unsigned int nNameLen;
	char* pszName;
	location_t* pLocation;
	FmVec3 vBoundMin;
	FmVec3 vBoundMax;
	unsigned int nMaterialCount;
	node_material_t* Materials;
	unsigned int nChildNodeCount;
	model_node_t* ChildNodes;
	int nIndex;

};

// 模型数据
struct model_t
{
	unsigned int nFlag;
	unsigned int nVersion;
	unsigned int nStartFrame;
	unsigned int nEndFrame;
	unsigned int nRootNodeCount;
	model_node_t* RootNodes;
	unsigned int nNodeCount;
	model_node_t** pNodeList;
	unsigned int nMaterialCount; // Specified how many material in current model
	unsigned int nLODCount;
	unsigned int nCurLODLevel;
};

#endif