
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


// �ڵ����
struct node_material_t
{
	unsigned int nVertexCount;
	vertex_data_t SingleVB;				//������Ϣ
	IStaticIB* pGPUIB;					//���㻺��
	IStaticVB* pSingleGPUVB;
	material_info_t MatInfo; // ��������
};

// ģ�ͽڵ�
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
	unsigned int nMaterialCount; // Specified how many material in current model
	unsigned int nLODCount;
	unsigned int nCurLODLevel;
};

#endif