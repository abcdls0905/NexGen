//--------------------------------------------------------------------
// 文件名:		model_player_shader.cpp
// 内  容:		
// 说  明:		
// 创建日期:	2009年5月4日
// 创建人:		陆利民
// 版权所有:	苏州蜗牛电子有限公司
//--------------------------------------------------------------------

#include "model_player.h"
#include "../visual/i_render.h"
#include "../utils/array_pod.h"
#include "../visual/shader_list.h"
#include "../utils/pod_hash_map.h" 
//#include <map>

extern IRender* g_pRender;

ShaderManager * ShaderManager::s_inst = NULL;

#define SET_VERTEX_DEFINE(vs_list) \
	vs_list.SetDefine(VERTEX_COLOR_SHIFT, "#define VERTEX_COLOR 1\n"); \
	vs_list.SetDefine(VERTEX_TANGENT_SHIFT, "#define VERTEX_TANGENT 1\n"); \
	vs_list.SetDefine(VERTEX_BINORMAL_SHIFT, "#define VERTEX_BINORMAL 1\n"); \
	vs_list.SetDefine(VERTEX_TEXTURE0_SHIFT, "#define VERTEX_TEXTURE0 1\n"); \
	vs_list.SetDefine(VERTEX_TEXTURE1_SHIFT, "#define VERTEX_TEXTURE1 1\n"); \
	vs_list.SetDefine(VERTEX_ANIMATION_SHIFT, "#define VERTEX_ANIMATION 1\n"); \
	vs_list.SetDefine(VERTEX_SKELETON_SHIFT, "#define VERTEX_SKELETON 1\n"); \
	vs_list.SetDefine(VERTEX_INSTANCE_SHIFT, "#define VERTEX_INSTANCE 1\n");

enum MODEL_VS_SHIFT_ENUM
{
	MODEL_VS_TEXTUREANI = VERTEX_SHIFT_MAX,
	MODEL_VS_DIFFUSEMAP,
	MODEL_VS_NORMALMAP,
	MODEL_VS_SPECULARMAP,
	MODEL_VS_DISAPPEAR,
	MODEL_VS_VIEWVECTOR,
	MODEL_VS_GAMMA,
	MODEL_VS_CLIPPLANE,
	MODEL_VS_FILTER,
	MODEL_VS_KEEPOUT,
	MODEL_VS_SHADOWMAP,
	MODEL_VS_HEIGHT_FOG,
	MODEL_VS_FOGEXP,
	MODEL_VS_FOGLINEAR,
	MODEL_VS_FILTERTEXTUREANI,
	MODEL_VS_MAX,
};

enum MODEL_PS_SHIFT_ENUM
{
	MODEL_PS_DIFFUSE,
	MODEL_PS_GREATERQUAL,
	MODEL_PS_DIFFUSEMAP,
	MODEL_PS_NORMALMAP,
	MODEL_PS_SPECULAR,
	MODEL_PS_SPECULARMAP,
	MODEL_PS_EMISSIVE,
	MODEL_PS_EMISSIVEMAP,
	MODEL_PS_LIGHTMAP,
	MODEL_PS_REFLECTION,
	//MODEL_PS_VOLUMETEXTURE,
	MODEL_PS_DYNAMICLIGHTING,
	MODEL_PS_SHADOWMAP,
	//MODEL_PS_PRELIGHT,
	MODEL_PS_FIXSHADOW,
	MODEL_PS_SKELETON,
	MODEL_PS_DISAPPEAR,
	MODEL_PS_OPACITY,
	MODEL_PS_APPENDCOLOR,
	MODEL_PS_FALLOFF,
	MODEL_PS_SPHEREAMBIENT,
	MODEL_PS_SKINEFFECT,
	MODEL_PS_FOGEXP,
	MODEL_PS_FOGLINEAR,
	MODEL_PS_ALPHATEST,
	//MODEL_PS_CLIPBLEND,
	MODEL_PS_CLIPSOLID,
	MODEL_PS_CLIPPLANE,
	MODEL_PS_ONLYZPASS,
	MODEL_PS_ONLYCOLOR,
	MODEL_PS_POINTLIGHT,
	MODEL_PS_FILTER,
	MODEL_PS_KEEPOUT,
	MODEL_PS_HEIGHT_FOG,
	MODEL_PS_BLEND_ENHANCE,
	MODEL_PS_MAX,
};

enum GBUFFER_VS_SHIFT_ENUM
{
	GBUFFER_VS_TEXTUREANI = VERTEX_SHIFT_MAX,
	GBUFFER_VS_NORMALMAP,
	GBUFFER_VS_FALLOFF,
	GBUFFER_VS_GAMMA,
	GBUFFER_VS_MAX,
};

enum GBUFFER_TESS_VS_SHIFT_ENUM
{
	GBUFFER_TESS_VS_TEXTUREANI = VERTEX_SHIFT_MAX,
	GBUFFER_TESS_VS_NORMALMAP,
	GBUFFER_TESS_VS_GAMMA,
	GBUFFER_TESS_VS_MAX,
};

enum GBUFFER_HS_SHIFT_ENUM
{
	GBUFFER_HS_NORMALMAP,
	GBUFFER_HS_VERTEX_COLOR,
	GBUFFER_HS_VERTEX_TEXTURE0,
	GBUFFER_HS_MAX,
};

enum GBUFFER_DS_SHIFT_ENUM
{
	GBUFFER_DS_NORMALMAP,
	GBUFFER_DS_VERTEX_COLOR,
	GBUFFER_DS_VERTEX_TEXTURE0,
	GBUFFER_DS_FALLOFF,
	GBUFFER_DS_MAX,
};

enum GBUFFER_PS_SHIFT_ENUM
{
	GBUFFER_PS_DIFFUSE,
	GBUFFER_PS_DIFFUSEMAP,
	GBUFFER_PS_NORMALMAP,
	GBUFFER_PS_SPECULARMAP,
	GBUFFER_PS_VOLUMETEXTURE,
	GBUFFER_PS_FALLOFF,
	GBUFFER_PS_APPENDCOLOR,
	GBUFFER_PS_SATURATION,
	GBUFFER_PS_ALPHATEST,
	GBUFFER_PS_CLIPBLEND,
	GBUFFER_PS_MAX,
};

enum DEPTH_VS_SHIFT_ENUM
{
	DEPTH_VS_ALPHATEST = VERTEX_SHIFT_MAX,
	DEPTH_VS_MAX,
};

enum DEPTH_PS_SHIFT_ENUM
{
	DEPTH_PS_ALPHATEST,
	DEPTH_PS_MAX,
};

enum SHADOWMAP_VS_SHIFT_ENUM
{
	SHADOWMAP_VS_ALPHATEST = VERTEX_SHIFT_MAX,
	SHADOWMAP_VS_MAX,
};

enum SHADOWMAP_TESS_VS_SHIFT_ENUM
{
	SHADOWMAP_TESS_VS_ALPHATEST = VERTEX_SHIFT_MAX,
	SHADOWMAP_TESS_VS_MAX,
};

enum SHADOWMAP_HS_SHIFT_ENUM
{
	SHADOWMAP_HS_ALPHATEST,
	SHADOWMAP_HS_MAX,
};

enum SHADOWMAP_DS_SHIFT_ENUM
{
	SHADOWMAP_DS_ALPHATEST,
	SHADOWMAP_DS_MAX,
};

enum SHADOWMAP_PS_SHIFT_ENUM
{
	SHADOWMAP_PS_ALPHATEST,
	SHADOWMAP_PS_MAX,
};

enum APPLIQUE_VS_SHIFT_ENUM
{
	APPLIQUE_VS_FOGDEPTH = VERTEX_SHIFT_MAX,
	APPLIQUE_VS_MAX,
};

enum APPLIQUE_PS_SHIFT_ENUM
{
	APPLIQUE_PS_VOLUMETEXTURE,
	APPLIQUE_PS_SHADOWMAP,
	APPLIQUE_PS_PRELIGHT,
	APPLIQUE_PS_DISAPPEAR,
	APPLIQUE_PS_OPACITY,
	APPLIQUE_PS_APPENDCOLOR,
	APPLIQUE_PS_FOGEXP,
	APPLIQUE_PS_FOGLINEAR,
	APPLIQUE_PS_USEINTZ,
	APPLIQUE_PS_MULTIPLY,
	APPLIQUE_PS_STEREO,
	APPLIQUE_PS_MAX,
};

enum REFRACTION_VS_SHIFT_ENUM
{
	REFRACTION_VS_TEXTUREANI = VERTEX_SHIFT_MAX,
	REFRACTION_VS_NORMALMAP,
	REFRACTION_VS_FALLOFF,
	REFRACTION_VS_MAX,
};

enum REFRACTION_PS_SHIFT_ENUM
{
	REFRACTION_PS_DIFFUSE,
	REFRACTION_PS_DIFFUSEMAP,
	REFRACTION_PS_NORMALMAP,
	REFRACTION_PS_VOLUMETEXTURE,
	REFRACTION_PS_OPACITY,
	REFRACTION_PS_MAX,
};

enum WATER_VS_SHIFT_ENUM
{
	WATER_VS_TEXTUREANI = VERTEX_SHIFT_MAX,
	WATER_VS_NORMALMAP,
	WATER_VS_VIEWVECTOR,
	WATER_VS_GAMMA,
	WATER_VS_MAX,
};

enum WATER_PS_SHIFT_ENUM
{
	WATER_PS_DIFFUSE,
	WATER_PS_DIFFUSEMAP,
	WATER_PS_NORMALMAP,
	WATER_PS_SPECULAR,
	WATER_PS_SPECULARMAP,
	WATER_PS_EMISSIVE,
	WATER_PS_EMISSIVEMAP,
	WATER_PS_REFLECTION,
	WATER_PS_OPACITY,
	WATER_PS_DEPTHMAP,
	WATER_PS_FOGEXP,
	WATER_PS_FOGLINEAR,
	WATER_PS_MAX,
};

enum VOLUMEFOG_VS_SHIFT_ENUM
{
	VOLUMEFOG_VS_FOGDEPTH = VERTEX_SHIFT_MAX,
	VOLUMEFOG_VS_MAX,
};

enum VOLUMEDEPTH_PS_SHIFT_ENUM
{
	VOLUMEDEPTH_PS_CW,
	VOLUMEDEPTH_PS_MAX,
};

enum VOLUMECOLOR_PS_SHIFT_ENUM
{
	VOLUMECOLOR_PS_APPENDCOLOR,
	VOLUMECOLOR_PS_OPACITY,
	VOLUMECOLOR_PS_FOGEXP,
	VOLUMECOLOR_PS_FOGLINEAR,
	VOLUMECOLOR_PS_MAX,
};

enum GLOW_VS_SHIFT_ENUM
{
	GLOW_VS_TEXTUREANI = VERTEX_SHIFT_MAX,
	GLOW_VS_FOGDEPTH,
	GLOW_VS_GAMMA,
	GLOW_VS_MAX,
};

enum GLOW_PS_SHIFT_ENUM
{
	GLOW_PS_DIFFUSE,
	GLOW_PS_DIFFUSEMAP,
	GLOW_PS_DYNAMICLIGHTING,
	GLOW_PS_APPENDCOLOR,
	GLOW_PS_EMISSIVE,
	GLOW_PS_EMISSIVEMAP,
	GLOW_PS_FOGEXP,
	GLOW_PS_FOGLINEAR,
	GLOW_PS_MAX,
};

static TShaderManyDefines<IVertexShader, MODEL_VS_MAX> s_VSList;
static TShaderManyDefines<IPixelShader, MODEL_PS_MAX> s_PSList;
static TShaderList<IVertexShader, GBUFFER_VS_MAX> s_GBufferVSList;
static TShaderList<IVertexShader, GBUFFER_TESS_VS_MAX> s_GBufferTessVSList;
//static TShaderList<IDxHullShader, GBUFFER_HS_MAX> s_GBufferHSList;
//static TShaderList<IDxDomainShader, GBUFFER_DS_MAX> s_GBufferDSList;
static TShaderList<IPixelShader, GBUFFER_PS_MAX> s_GBufferPSList;
static TShaderList<IVertexShader, DEPTH_VS_MAX> s_DepthVSList;
static TShaderList<IPixelShader, DEPTH_PS_MAX> s_DepthPSList;
static TShaderManyDefines<IVertexShader, SHADOWMAP_VS_MAX> s_ShadowMapVSList;
static TShaderList<IVertexShader, SHADOWMAP_TESS_VS_MAX> s_ShadowMapTessVSList;
//static TShaderList<IDxHullShader, SHADOWMAP_HS_MAX> s_ShadowMapHSList;
//static TShaderList<IDxDomainShader, SHADOWMAP_DS_MAX> s_ShadowMapDSList;
static TShaderManyDefines<IPixelShader, SHADOWMAP_PS_MAX> s_ShadowMapPSList;
static TShaderList<IVertexShader, APPLIQUE_VS_MAX> s_AppliqueVSList;
static TShaderList<IPixelShader, APPLIQUE_PS_MAX> s_AppliquePSList;
static TShaderList<IVertexShader, REFRACTION_VS_MAX> s_RefractionVSList;
static TShaderList<IPixelShader, REFRACTION_PS_MAX> s_RefractionPSList;
static TShaderList<IVertexShader, WATER_VS_MAX> s_WaterVSList;
static TShaderList<IPixelShader, WATER_PS_MAX> s_WaterPSList;
static TShaderList<IVertexShader, VOLUMEFOG_VS_MAX> s_VolumeFogVSList;
static TShaderList<IPixelShader, VOLUMEDEPTH_PS_MAX> s_VolumeFogDepthPSList;
static TShaderList<IPixelShader, VOLUMECOLOR_PS_MAX> s_VolumeFogColorPSList;
static TShaderList<IVertexShader, GLOW_VS_MAX> s_GlowVSList;
static TShaderList<IPixelShader, GLOW_PS_MAX> s_GlowPSList;

void shader_initialize()
{
	// 设置着色器

	//g_pVS = g_pRender->LoadVertexShader("model.vsh", "main", "");
	//g_pPS = g_pRender->LoadPixelShader("model.fsh", "main", "");
	//g_pShader = g_pRender->CreateShaderProgram(g_pVS, g_pPS, vertex_t_name ,5);

	s_VSList.SetRender(g_pRender);
	s_VSList.SetShaderName("model.vsh", "main");
	SET_VERTEX_DEFINE(s_VSList);

	s_VSList.SetDefine(MODEL_VS_TEXTUREANI, "#define TEXTUREANI 1\n");
	s_VSList.SetDefine(MODEL_VS_DIFFUSEMAP, "#define DIFFUSEMAP 1\n");
	s_VSList.SetDefine(MODEL_VS_NORMALMAP, "#define NORMALMAP 1\n");
	s_VSList.SetDefine(MODEL_VS_SPECULARMAP, "#define SPECULARMAP 1\n");
	s_VSList.SetDefine(MODEL_VS_DISAPPEAR, "#define DISAPPEAR 1\n");
	s_VSList.SetDefine(MODEL_VS_VIEWVECTOR, "#define VIEWVECTOR 1\n");
	s_VSList.SetDefine(MODEL_VS_GAMMA, "#define GAMMA 1\n");
	s_VSList.SetDefine(MODEL_VS_CLIPPLANE, "#define CLIPPLANE 1\n");
	s_VSList.SetDefine(MODEL_VS_FILTER, "#define FILTERMAP 1\n");
	s_VSList.SetDefine(MODEL_VS_KEEPOUT, "#define KEEPOUT 1\n");
	s_VSList.SetDefine(MODEL_VS_SHADOWMAP, "#define SHADOWMAP 1\n");
	s_VSList.SetDefine(MODEL_VS_FOGEXP, "#define FOGEXP 1\n");
	s_VSList.SetDefine(MODEL_VS_FOGLINEAR, "#define FOGLINEAR 1\n");
	s_VSList.SetDefine(MODEL_VS_HEIGHT_FOG, "#define HEIGHT_FOG 1\n");
	s_VSList.SetDefine(MODEL_VS_FILTERTEXTUREANI, "#define FILTERTEXTUREANI 1\n");
	
	
	//s_VSList.LoadShader(0);
	
	s_PSList.SetRender(g_pRender);
	s_PSList.SetShaderName("model.fsh", "main");
	s_PSList.SetDefine(MODEL_PS_DIFFUSE, "#define DIFFUSE 1\n");
	s_PSList.SetDefine(MODEL_PS_GREATERQUAL, "#define ALPHATEST_GREATERQUAL 1\n");
	s_PSList.SetDefine(MODEL_PS_DIFFUSEMAP, "#define DIFFUSEMAP 1\n");
	s_PSList.SetDefine(MODEL_PS_NORMALMAP, "#define NORMALMAP 1\n");
	s_PSList.SetDefine(MODEL_PS_SPECULAR, "#define SPECULAR 1\n");
	s_PSList.SetDefine(MODEL_PS_SPECULARMAP, "#define SPECULARMAP 1\n");
	s_PSList.SetDefine(MODEL_PS_EMISSIVE, "#define EMISSIVE 1\n");
	s_PSList.SetDefine(MODEL_PS_EMISSIVEMAP, "#define EMISSIVEMAP 1\n");
	s_PSList.SetDefine(MODEL_PS_LIGHTMAP, "#define LIGHTMAP 1\n");
	s_PSList.SetDefine(MODEL_PS_REFLECTION, "#define REFLECTION 1\n");
//	s_PSList.SetDefine(MODEL_PS_VOLUMETEXTURE, "#define VOLUMETEXTURE 1\n");
	s_PSList.SetDefine(MODEL_PS_DYNAMICLIGHTING, "#define DYNAMICLIGHTING 1\n");
	s_PSList.SetDefine(MODEL_PS_SHADOWMAP, "#define SHADOWMAP 1\n");
//	s_PSList.SetDefine(MODEL_PS_PRELIGHT, "#define PRELIGHT 1\n");
	s_PSList.SetDefine(MODEL_PS_FIXSHADOW, "#define FIXSHADOW 1\n");
	s_PSList.SetDefine(MODEL_PS_SKELETON, "#define SKELETON 1\n");
	s_PSList.SetDefine(MODEL_PS_DISAPPEAR, "#define DISAPPEAR 1\n");
	s_PSList.SetDefine(MODEL_PS_OPACITY, "#define OPACITY 1\n");
	s_PSList.SetDefine(MODEL_PS_APPENDCOLOR, "#define APPENDCOLOR 1\n");
	s_PSList.SetDefine(MODEL_PS_FALLOFF, "#define FALLOFF 1\n");
	s_PSList.SetDefine(MODEL_PS_SPHEREAMBIENT, "#define SPHEREAMBIENT 1\n");
	s_PSList.SetDefine(MODEL_PS_SKINEFFECT, "#define SKINEFFECT 1\n");
	s_PSList.SetDefine(MODEL_PS_FOGEXP, "#define FOGEXP 1\n");
	s_PSList.SetDefine(MODEL_PS_FOGLINEAR, "#define FOGLINEAR 1\n");
	s_PSList.SetDefine(MODEL_PS_ALPHATEST, "#define ALPHATEST 1\n");
	//s_PSList.SetDefine(MODEL_PS_CLIPBLEND, "#define CLIPBLEND 1\n");
	s_PSList.SetDefine(MODEL_PS_CLIPSOLID, "#define CLIPSOLID 1\n");
	s_PSList.SetDefine(MODEL_PS_CLIPPLANE, "#define CLIPPLANE 1\n");
	s_PSList.SetDefine(MODEL_PS_ONLYZPASS, "#define ONLYZPASS 1\n");
	s_PSList.SetDefine(MODEL_PS_ONLYCOLOR, "#define ONLYCOLOR 1\n");
	s_PSList.SetDefine(MODEL_PS_POINTLIGHT, "#define POINTLIGHT 1\n");
	s_PSList.SetDefine(MODEL_PS_FILTER, "#define FILTERMAP 1\n");
	s_PSList.SetDefine(MODEL_PS_KEEPOUT, "#define KEEPOUT 1\n");
	s_PSList.SetDefine(MODEL_PS_HEIGHT_FOG, "#define HEIGHT_FOG 1\n");
	s_PSList.SetDefine(MODEL_PS_BLEND_ENHANCE, "#define BLEND_ENHANCE 1\n");

	s_ShadowMapVSList.SetRender(g_pRender);
	s_ShadowMapVSList.SetShaderName("model_shadow_map.vsh", "vs_main_model_shadow_map");
	SET_VERTEX_DEFINE(s_ShadowMapVSList);
	s_ShadowMapVSList.SetDefine(SHADOWMAP_VS_ALPHATEST, "#define ALPHATEST 1\n");

	s_ShadowMapPSList.SetRender(g_pRender);
	s_ShadowMapPSList.SetShaderName("model_shadow_map.fsh", "ps_main_model_shadow_map");
	s_ShadowMapPSList.SetDefine(SHADOWMAP_PS_ALPHATEST, "#define ALPHATEST 1\n");
	
}

void shader_release()
{
	ShaderManager::Inst().Destroy();
	s_VSList.Clear();
	s_PSList.Clear();
	s_ShadowMapVSList.Clear();
	s_ShadowMapPSList.Clear();
}

const char* const_value_name[c_max] = 
{
	"c_mtxWorld",
	"c_mtxWVP",
	"c_fBumpScale",
	"c_fReflectScale",
	"c_fSaturationScale",
	"c_fGlowAlpha",
	"c_fBlendWeight",
	"c_fAlphaRefValue",
	"c_fTime",
	"c_fGlowSize",
	"c_BoneList",
	"c_mtxDiffuseTM",
	"c_MaterialEmissive",
	"c_MaterialSpecular",
	"c_fSpecularPower",
	"c_MaterialDiffuse",
	"c_AppendColor",
	"c_MaterialAmbient",
	"c_MaterialAmbientEX",
	"c_vLightDir",
	"c_mtxViewInverse",
	"c_LightAmbient",
	"c_vViewPos",
	"c_vReferPos",
	"c_ClipPlane",
	"c_LightDiffuse",
	"c_FogColor",
	"c_FogParam",
	"c_FogExpParam",
	"c_FallOffParam",
	"c_SubSurfaceParam",
	"c_LightMapParam",
	"c_vLightMapColorScale",
	"c_PointLightDiffuse",
	"c_PointLightPosition",
	"c_FilterParam",
	"c_mtxFilterTM",
	"c_mtxShadowViewProj",
	"c_HeightFogColor",
	"c_HeightFogParam",
	"tex_Diffuse",
	"tex_Bump",
	"tex_SpecularMap",
	"tex_EmissiveMap",
	"tex_ReflectionMap",
	"tex_LightMap",
	"tex_FilterMap",
	"tex_KeepoutMap",
	"tex_Shadow",
};
