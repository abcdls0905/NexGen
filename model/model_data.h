
#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_

#include "../visual/i_resource.h"
#include "../visual/i_texture.h"
#include "../public/core_log.h"
#include "../visual/i_model_player.h"
#include "location.h"

// �������ݽṹ
enum VERTEX_SHIFT_ENUM
{
  VERTEX_COLOR_SHIFT,
  VERTEX_TANGENT_SHIFT,
  VERTEX_BINORMAL_SHIFT,
  VERTEX_TEXTURE0_SHIFT,
  VERTEX_TEXTURE1_SHIFT,
  VERTEX_ANIMATION_SHIFT,
  VERTEX_SKELETON_SHIFT,
  VERTEX_INSTANCE_SHIFT,
  VERTEX_SHIFT_MAX,
};

#define VERTEX_COLOR_FLAG		(0x1 << VERTEX_COLOR_SHIFT)
#define VERTEX_TANGENT_FLAG		(0x1 << VERTEX_TANGENT_SHIFT)
#define VERTEX_BINORMAL_FLAG	(0x1 << VERTEX_BINORMAL_SHIFT)
#define VERTEX_TEXTURE0_FLAG	(0x1 << VERTEX_TEXTURE0_SHIFT)
#define VERTEX_TEXTURE1_FLAG	(0x1 << VERTEX_TEXTURE1_SHIFT)
#define VERTEX_ANIMATION_FLAG	(0x1 << VERTEX_ANIMATION_SHIFT)
#define VERTEX_SKELETON_FLAG	(0x1 << VERTEX_SKELETON_SHIFT)
#define VERTEX_INSTANCE_FLAG	(0x1 << VERTEX_INSTANCE_SHIFT)

// ģ�ͱ����Ϣ
const unsigned int MODEL_POSITION_INFO		= 0x00000001; // ����λ����Ϣ���
const unsigned int MODEL_POSITION_ANI		= 0x00000002; // ����λ�ö�����Ϣ���
const unsigned int MODEL_NORMAL_INFO		= 0x00000004; // ����ķ�����Ϣ���
const unsigned int MODEL_NORMAL_ANI			= 0x00000008; // ����ķ��򶯻���Ϣ���
const unsigned int MODEL_COLOR_INFO			= 0x00000010; // ����ɫ��Ϣ���
const unsigned int MODEL_COLOR_ANI			= 0x00000020; // ����ɫ������Ϣ���
const unsigned int MODEL_ILLUM_INFO			= 0x00000040; // ����������Ϣ���
const unsigned int MODEL_ILLUM_ANI			= 0x00000080; // ��������������Ϣ���
const unsigned int MODEL_TANGENT_INFO		= 0x00000100; // ����������Ϣ���
const unsigned int MODEL_TANGENT_ANI		= 0x00000200; // �������߶�����Ϣ���
const unsigned int MODEL_BINORMAL_INFO		= 0x00000400; // ����η�����Ϣ���
const unsigned int MODEL_BINORMAL_ANI		= 0x00000800; // ����η��߶�����Ϣ���
const unsigned int MODEL_MATRIX_INFO		= 0x00001000; // ���󶯻���Ϣ���
const unsigned int MODEL_MATRIX_ANI			= 0x00002000; // ���󶯻���Ϣ���
const unsigned int MODEL_SKIN_FLAG			= 0x00010000; // ��Ƥ���
const unsigned int MODEL_MESH_FLAG			= 0x00020000; // ������
const unsigned int MODEL_DISAPPEAR_FLAG		= 0x00040000; // ���뵭�����
const unsigned int MODEL_SCENEFOG_FLAG		= 0x00080000; // ��������
const unsigned int MODEL_APPLIQUE_FLAG		= 0x00100000; // ����ģ�ͱ��
const unsigned int MODEL_BB_FLAG			= 0x00200000;
const unsigned int MODEL_BBX_FLAG			= 0x00400000;
const unsigned int MODEL_BBY_FLAG			= 0x00800000;
const unsigned int MODEL_TREELEAF_FLAG		= 0x01000000;
const unsigned int MODEL_HIDE_FLAG			= 0x02000000;
const unsigned int MODEL_BLEND_FLAG			= 0x04000000;
const unsigned int MODEL_BBEX_FLAG			= 0x08000000;
const unsigned int MODEL_MAINMODEL_FLAG		= 0x10000000;
const unsigned int MODEL_WATERCLIP_FLAG		= 0x20000000; // ˮ������ģ�ͱ��
const unsigned int MODEL_FLAG_EXT			= 0x80000000; // ģ�ͱ����չ���

const unsigned int HELPER_POSITION_INFO		= MODEL_POSITION_INFO; // ����λ����Ϣ���
const unsigned int HELPER_POSITION_ANI		= MODEL_POSITION_ANI; // ����λ�ö�����Ϣ���
const unsigned int HELPER_NORMAL_INFO		= MODEL_NORMAL_INFO; // ����ķ�����Ϣ���
const unsigned int HELPER_NORMAL_ANI		= MODEL_NORMAL_ANI; // ����ķ��򶯻���Ϣ���
const unsigned int HELPER_MATRIX_INFO		= MODEL_MATRIX_INFO; // ���󶯻���Ϣ���
const unsigned int HELPER_MATRIX_ANI		= MODEL_MATRIX_ANI; // ���󶯻���Ϣ���
const unsigned int HELPER_CAMERA_FLAG		= 0x00010000; // ������Ϊ������ı��
const unsigned int HELPER_FLAG_EXT			= 0x80000000; // ����������չ���

// ���ʱ����Ϣ
const unsigned int MATERIAL_AMBIENT_INFO			= 0x00000001; // ���ʻ����ⷴ����Ϣ���
const unsigned int MATERIAL_AMBIENT_ANI				= 0x00000002; // ���ʻ����ⷴ�䶯����Ϣ���
const unsigned int MATERIAL_DIFFUSE_INFO			= 0x00000004; // ������������Ϣ���
const unsigned int MATERIAL_DIFFUSE_ANI				= 0x00000008; // ���������䶯����Ϣ���
const unsigned int MATERIAL_SPECULAR_INFO			= 0x00000010; // ���ʾ��淴����Ϣ���
const unsigned int MATERIAL_SPECULAR_ANI			= 0x00000020; // ���ʾ��淴�䶯����Ϣ���
const unsigned int MATERIAL_EMISSIVE_INFO			= 0x00000040; // ���ʷ�����Ϣ���
const unsigned int MATERIAL_EMISSIVE_ANI			= 0x00000080; // ���ʷ��䶯����Ϣ���
const unsigned int MATERIAL_OPACITY_INFO			= 0x00000100; // ����͸����Ϣ���
const unsigned int MATERIAL_OPACITY_ANI				= 0x00000200; // ����͸��������Ϣ���
const unsigned int MATERIAL_GLOSS_INFO				= 0x00000400; // ����Glossiness��Ϣ���
const unsigned int MATERIAL_GLOSS_ANI				= 0x00000800; // ����Glossiness������Ϣ���
const unsigned int MATERIAL_DIFFUSE_MAP_INFO		= 0x00001000; // Diffuse��ͼ��Ϣ���
const unsigned int MATERIAL_BUMP_MAP_INFO			= 0x00002000; // Bump��ͼ��Ϣ���
const unsigned int MATERIAL_SPECULAR_MAP_INFO		= 0x00004000; // Specular��ͼ��Ϣ���
const unsigned int MATERIAL_SPECULAR_LEVEL_MAP_INFO	= 0x00008000; // Specularlevel��ͼ��Ϣ���
const unsigned int MATERIAL_GLOSSINESS_MAP_INFO		= 0x00010000; // Glossiness��ͼ��Ϣ���
const unsigned int MATERIAL_REFLECTION_ENV_MAP_INFO	= 0x00020000; // Reflection��ͼ��Ϣ���
const unsigned int MATERIAL_LIGHTMAP_INFO			= 0x00040000; // Light��ͼ��Ϣ���
const unsigned int MATERIAL_DIFFUSE_UV_ANI			= 0x00080000; // DIFFUSE_MAP��UV������Ϣ���
const unsigned int MATERIAL_SELF_ILLUMINATION_INFO	= 0x00100000; // �Է�����ͼ��Ϣ���
const unsigned int MATERIAL_ATEST_FLAG				= 0x00200000; // Alpha���Ա��
const unsigned int MATERIAL_BLEND_FLAG				= 0x00400000; // Alpha��ϱ��
const unsigned int MATERIAL_NOLIGHT_FLAG			= 0x00800000; // �޹��ձ��
const unsigned int MATERIAL_BLENDENHANCE_FLAG		= 0x01000000; // �������ӱ��
const unsigned int MATERIAL_REFRACTION_FLAG			= 0x02000000; // ������ʱ��
const unsigned int MATERIAL_NOZWRITE_FLAG			= 0x04000000; // �����д����
const unsigned int MATERIAL_DOUBLESIDE_FLAG			= 0x08000000; // ˫����ʱ��
const unsigned int MATERIAL_BLENDQUALITY			= 0x10000000; // ģ�Ͷ��λ��Ʊ��
const unsigned int MATERIAL_SAMPLER_CLAMP			= 0x20000000; // ��������CLAMPģʽ
const unsigned int MATERIAL_VOLUMEFOG_FLAG			= 0x40000000; // �������ʱ�ǩ
const unsigned int MATERIAL_FLAG_EXT				= 0x80000000; // ģ�Ͳ��ʱ����չ���

// ��չ���ʱ����Ϣ
const unsigned int MATERIAL_FRAME_INFO				=	0x00000001;	// ����֡������Ϣ���
const unsigned int MATERIAL_SHADOW_INFO				=	0x00000004;	// Ͷ����Ӱ���
const unsigned int MATERIAL_SHADOWED_INFO			=	0x00000008;	// ��ͶӰ���
const unsigned int MATERIAL_BLENDSHADOW_INFO		=	0x00000010;	// ��͸����Ӱ���
const unsigned int MATERIAL_SIEVE_INFO				=	0x00000020;	// ɸ�װ�͸�����
const unsigned int MATERIAL_FILTER_MAP_INFO			=	0x00000080;	// ������ͼ��Ϣ���
const unsigned int MATERIAL_FILTER_PARAM_ANI		=	0x00000100;	// ���ֲ����������
const unsigned int MATERIAL_FILTER_UV_ANI			=	0x00000200;	// ������ͼ��UV������Ϣ���


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

#include "model_vf.h"


// ����Ķ�����Ƥ����
struct cache_skin_t
{
  int nSkinSequenceId;
  FmVec3* pSkinVectors;
  unsigned int nSkinVectorNum;
};

// ����Ĺ�����������
struct cache_bone_t
{
  int nBoneSequenceId;
  FmVec4* pBoneMatrices;
  unsigned int nBoneMatrixNum;
};


// ��ͼ����
struct texture_info_t
{
	char* pFileName; // ��ͼ�ļ��� 
	unsigned int nFileNameLen;
	ITexture* pTex; // ��ͼ��Դָ��
};

struct filter_param
{
	float fScale;
	float fBias;
	float fScaleAlpha;
	float fBiasAlpha;
};

// ��������
struct material_info_t
{
	FmVec3	vAmbient;	// ������
	FmVec3	vAmbientEx; //��������չ
	FmVec3 vDiffuse;	// ������
	FmVec3 vSpecular;	// �߹ⷴ��
	FmVec3 vEmissive;	// �Է���	

	float fGlossiness;		// �����
	float fOpacity;			// ͸����
	float fAlphaRef;		// ALPHA���Բο�ֵ
	float fSubSurfaceScale;	// �α���ɢ�䷶Χ
	float fSubSurfaceFactor;// �α���ǿ������
	float fReflectFactor;	// ��������ǿ��
	float fBumpScale;		// ����ǿ��
	float fSaturationScale;	// ���ʱ��Ͷ�
	float fHeightScale;		// ����ϸ�ָ߶�

	filter_param filterParam; // ���ֲ���

	// ����Ч������
	bool bSpecularEnable;	// �߹⿪��
	bool bEmissiveEnable;	// �Է��⿪��
	bool bOpacityEnable;	// ͸������
	bool bReflectEnable;	// ʹ�÷�����ͼ
	bool bBumpMapEnable;	// ʹ�÷�����ͼ
	bool bTessellationEnable; // ʹ�ñ���ϸ��
	bool bFilterEnable;		// ʹ��������ͼ

	// ��Ⱦ״̬
	bool bAlphaTest;		// ALPHA����
	bool bAlphaTestGreaterQual;	//ALPHA���Ժ����Ƿ�ΪGREATERQUAL
	bool bBlend;			// ALPHA���
	bool bBlendEnhance;		// ����ALPHA���
	bool bBlendQuality;		// ������Ⱦ���������
	bool bNoZWrite;			// ��д����Ȼ���
	bool bNoZTest;			// ����Ȳ���
	bool bDoubleSide;		// ˫����ʾ
	bool bVolumeTexture;	// �������
	bool bBlendDownLevel;	// ��͸�����彵����ʾ

	// ������ʽ
	bool bSamplerClamp;		// ������������ͣ���ڱ߽���

	// ����Ч����ʾ
	bool bUIFloat;			// ���ڽ��������
	bool bNoLight;			// ���趯̬����
	bool bSphereAmbient;	// ���װ������	
	bool bDisappear;		// ���뵭����͸�����뿪����
	bool bSkinEffect;		// Ƥ��Ч��
	bool bHairEffect;		// ë��Ч��

	// ����Ч��
	bool bGlowEnable;		// ���⿪��
	bool bGlowEntire;		// �Ƿ����巺��
	bool bGlowEmissive;		// �Է�����ɫ����
	bool bGlowEmissiveMap;	// �Է�����ͼ����
	bool bGlowDiffuse;		// �����䷺��
	bool bGlowVertexColor;	// ����ɫ����
	bool bGlowAppendColor;	// ����ɫ����
	bool bGlowEnhance;		// ��������
	bool bGlowBlur;			// ����ģ��
	float fGlowSize;		// �����С
	float fGlowAlpha;		// ����͸����

	// �����⣨ʹ���Է�����ɫ��
	bool bRimLight;			// �����⿪��
	FmVec3 vRimColor;		// ��������ɫ

	float fRimIntensity;	// ����������
	float fRimPower;		// ������ָ��

	// ר����;��ģ��
	bool bRefraction;		// ��ĻŤ�������䣩
	bool bApplique;			// ģ������
	bool bSceneFog;			// ���������
	bool bForbidStandModel;	// ��ֹվ����ײ����ģ�ͣ����ɼ���
	bool bPhaseMonitor;		// ʹ����λ����
	//unsigned int nShaderInfo;
	texture_info_t DiffuseMap;			// ��������ͼ
	texture_info_t BumpMap;				// ������ͼ
	texture_info_t LightMap;			// ����ͼ
	texture_info_t SpecularMap;			// �߹���ͼ
	texture_info_t SpecularLevelMap;	// �߹�������ͼ	
	texture_info_t GlossinessMap;		// �������ͼ
	texture_info_t EmissiveMap;			// �Է�����ͼ
	texture_info_t ReflectionMap;		// ������ͼ
	texture_info_t DisplacementMap;		// λ����ͼ
	texture_info_t FilterMap;			// ������ͼ
};


// Ϊģ������ר��ָ���Ĳ��ʲ���
struct custom_material_t
{
  const char* pMaterialName;			// ������
  material_info_t MatInfo;			// ��������
};


// �ڵ����
struct node_material_t
{
  ModelVF vf;
  char* pszName;
	unsigned int nVertexCount;
	vertex_data_t SingleVB;				//������Ϣ
	IStaticIB* pGPUIB;					  //���㻺��
	IStaticVB* pSingleGPUVB;
	material_info_t MatInfo;      // ��������
  unsigned short* indices;
  unsigned int nIndicesCount;
  unsigned int nSingleVertexSize;
  unsigned int nMaterialInfo;
  unsigned int nMaterialInfoEx;
  unsigned int nVertexDefine;
};

// ģ�ͽڵ�
struct model_node_t
{
  FXNODE_TYPE nType;
	unsigned int nNameLen;
	char* pszName;
	location_t* pLocation;
	FmVec3 vBoundMin;
	FmVec3 vBoundMax;
	unsigned int nMaterialCount;
	node_material_t* Materials;
	unsigned int nChildNodeCount;
	model_node_t* ChildNodes;
  unsigned int nModelInfo;
  int nIndex;

};

// ģ������
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
  char* pszTexBase;
	unsigned int nMaterialCount; // Specified how many material in current model
	unsigned int nLODCount;
	unsigned int nCurLODLevel;
};


//MD2 temp struct
typedef struct
{
  int ident;		// identifies as MD2 file "IDP2"
  int version;		// mine is 8
  int skinwidth;    // width of texture
  int skinheight;   // height of texture
  int framesize;    // number of bytes per frame
  int numSkins;     // number of textures
  int numXYZ;       // number of points
  int numST;        // number of texture
  int numTris;      // number of triangles
  int numGLcmds;
  int numFrames;    // total number of frames
  int offsetSkins;  // offset to skin names (64 bytes each)
  int offsetST;     // offset of texture s-t values
  int offsetTris;   // offset of triangle mesh
  int offsetFrames; // offset of frame data (points)
  int offsetGLcmds; // type of OpenGL commands to use
  int offsetEnd;    // end of file
} ModelHeader_t;

class Md2Vertex
{
public:
  unsigned char position[3];
  unsigned char normalIndex;
};

class Md2Frame
{
public:
  float scale[3];
  float translation[3];
  char name[16];
  Md2Vertex vertices[1];	//start of vertex list for this frame
};

class Md2TexCoord
{
public:
  short s, t;
};

class Md2Triangle
{
public:
  unsigned short vertexIndices[3];
  unsigned short stIndices[3];
};

// ��¼��־��Ϣ
static void LOG_ERROR(const char* format, ...)
{
  char buf[256] = { 0 };
  va_list	vlist;

  va_start(vlist, format);

  if (vsnprintf(buf, sizeof(buf) - 1, format, vlist) < 0)
  {
    buf[sizeof(buf) - 1] = 0;
  }

  va_end(vlist);
  CORE_TRACE(buf);
}


// ����ģ��ʵ��
model_t* load_model(const char* pszModelName, char* tex = 0);
// ж��ģ��ʵ��
void unload_model(model_t* pModel);

#endif