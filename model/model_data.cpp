

#include "model_data.h"

#include "../visual/i_texture.h"
#include "../visual/vis_utils.h"
#include "../public/inlines.h"
#include "../public/core_file.h"
#include "../public/core_log.h"
#include "../visual/i_render.h"

#include "../render/render_header.h"

extern IRender* g_pRender;

// 初始化材质数据
void init_material_value(material_info_t* pMatInfo);

struct node_key_index_t
{
  const char* strName;
  unsigned int nHash;
  int nDefine;
  bool bAllowModify;
};

// 节点键索引表
static node_key_index_t s_NodeKeyIndex[] = {
  "PositionInfo", 0, MODEL_POSITION_INFO, false,
  "PositionAni", 0, MODEL_POSITION_ANI, false,
  "NormalInfo", 0, MODEL_NORMAL_INFO, false,
  "NormalAni", 0, MODEL_NORMAL_ANI, false,
  "ColorInfo", 0, MODEL_COLOR_INFO, false,
  "ColorAni", 0, MODEL_COLOR_ANI, false,
  "IllumInfo", 0, MODEL_ILLUM_INFO, false,
  "IllumAni", 0, MODEL_ILLUM_ANI, false,
  "TangentInfo", 0, MODEL_TANGENT_INFO, false,
  "TangentAni", 0, MODEL_TANGENT_ANI, false,
  "BinormalInfo", 0, MODEL_BINORMAL_INFO, false,
  "BinormalAni", 0, MODEL_BINORMAL_ANI, false,
  "MatrixInfo", 0, MODEL_MATRIX_INFO, false,
  "MatrixAni", 0, MODEL_MATRIX_ANI, false,
  "Skin", 0, MODEL_SKIN_FLAG, false,
  "Mesh", 0, MODEL_MESH_FLAG, false,
  "Disappear", 0, MODEL_DISAPPEAR_FLAG, true,
  "SceneFog", 0, MODEL_SCENEFOG_FLAG, true,
  "Applique", 0, MODEL_APPLIQUE_FLAG, true,
  "BB", 0, MODEL_BB_FLAG, true,
  "BBX", 0, MODEL_BBX_FLAG, true,
  "BBY", 0, MODEL_BBY_FLAG, true,
  "TreeLeaf", 0, MODEL_TREELEAF_FLAG, true,
  "Hide", 0, MODEL_HIDE_FLAG, true,
  "Blend", 0, MODEL_BLEND_FLAG, true,
  "BBEX", 0, MODEL_BBEX_FLAG, true,
  "MainModel", 0, MODEL_MAINMODEL_FLAG, true,
  "WaterClip", 0, MODEL_WATERCLIP_FLAG, true,
  "HelperPositionInfo", 0, HELPER_POSITION_INFO, false,
  "HelperPositionAni", 0, HELPER_POSITION_ANI, false,
  "HelperNormalInfo", 0, HELPER_NORMAL_INFO, false,
  "HelperNormalAni", 0, HELPER_NORMAL_ANI, false,
  "HelperMatrixInfo", 0, HELPER_MATRIX_INFO, false,
  "HelperMatrixAni", 0, HELPER_MATRIX_ANI, false,
  "HelperCamera", 0, HELPER_CAMERA_FLAG, false,
};

static inline bool read_file(FILE* fp, void* buffer, size_t size)
{
  return core_file::fread(buffer, sizeof(char), size, fp) == size;
}


// 初始化节点键名索引表
void init_node_key_index()
{
  for (int i = 0; i < sizeof(s_NodeKeyIndex) / sizeof(node_key_index_t); ++i)
  {
    s_NodeKeyIndex[i].nHash = GetHashValue(s_NodeKeyIndex[i].strName);
  }
}

FmVec3 CalculateNormal(float *p1, float *p2, float *p3)
{
  float a[3], b[3], result[3];
  float length;

  a[0] = p1[0] - p2[0];
  a[1] = p1[1] - p2[1];
  a[2] = p1[2] - p2[2];

  b[0] = p1[0] - p3[0];
  b[1] = p1[1] - p3[1];
  b[2] = p1[2] - p3[2];

  result[0] = a[1] * b[2] - b[1] * a[2];
  result[1] = b[0] * a[2] - a[0] * b[2];
  result[2] = a[0] * b[1] - b[0] * a[1];

  // calculate the length of the normal
  length = (float)sqrt(result[0]*result[0] + result[1]*result[1] + result[2]*result[2]);

  // normalize and specify the normal
  return FmVec3(result[0] / length, result[1] / length, result[2] / length);
}

model_t* load_md2_model(const char* pszModelName)
{
  //CORE_LOG_DEVELOPER("[START] LoadVertexAniModel(%s)\n", pszModelName);
  FILE* pFile = core_file::fopen(pszModelName, "rb");

  if (NULL == pFile)
  {
    LOG_ERROR("[error 030008](load_model)can't open model file! "
      "file name %s", pszModelName);
    return NULL;
  }
  ModelHeader_t header;
  model_t* ret = (model_t*)CORE_ALLOC(sizeof(model_t));
  memset(ret, 0, sizeof(model_t));

  // header
  if (!read_file(pFile, &header, sizeof(ModelHeader_t)))
  {
    LOG_ERROR("(load_model)model flag read failed, file %s", pszModelName);
    CORE_FREE(ret, sizeof(model_t));
    core_file::fclose(pFile);
    return NULL;
  }

  //load vertexs
  int numVertices = header.numXYZ;
  FmVec3* vertexs = new FmVec3[header.numXYZ * header.numFrames];
  unsigned char * frameBuffer=new unsigned char[header.framesize];
  for(int i=0; i<header.numFrames; ++i)
  {
    fseek(pFile, header.offsetFrames+i*header.framesize, SEEK_SET);
    fread(frameBuffer, header.framesize, 1, pFile);
    Md2Frame * framePtr=(Md2Frame *) frameBuffer;
    for(int j=0; j<header.numXYZ; ++j)
    {
      vertexs[i*numVertices+j].x= float(framePtr->scale[0] * framePtr->vertices[j].position[0] + framePtr->translation[0]);
      vertexs[i*numVertices+j].y= float(framePtr->scale[2] * framePtr->vertices[j].position[2] + framePtr->translation[2]);
      vertexs[i*numVertices+j].z=-float(framePtr->scale[1] * framePtr->vertices[j].position[1] + framePtr->translation[1]);
      //vertexs[i*numVertices+j]/=64;
    }
  }

  //load textures
  Md2TexCoord* texCoords = new Md2TexCoord[header.numST];
  fseek(pFile, header.offsetST, SEEK_SET);
  fread(texCoords, header.numST, sizeof(Md2TexCoord), pFile);

  //load triangles
  int numTriangles=header.numTris;
  Md2Triangle* triangles=new Md2Triangle[numTriangles];
  fseek(pFile, header.offsetTris, SEEK_SET);
  fread(triangles, numTriangles, sizeof(Md2Triangle), pFile);

  //build VertexBuffer and IndexBuffer

  ret->nRootNodeCount = 1;
  ret->RootNodes = (model_node_t*)CORE_ALLOC(
    sizeof(model_node_t) * ret->nRootNodeCount);

  for (int i = 0; i < ret->nRootNodeCount; ++i)
  {
    model_node_t* root = &ret->RootNodes[i];
    root->nMaterialCount = 1;
    root->Materials = new node_material_t[root->nMaterialCount];
    for (int j = 0; j < root->nMaterialCount; ++j)
    {
      node_material_t* material = &root->Materials[j];
      init_material_value(&material->MatInfo);
      //关键构造
      material->pGPUIB;
      material->pSingleGPUVB;
      material->MatInfo;

      //只取第一帧静态数据
      //build vb
      FmVec3* startFrame = &vertexs[0];
      int trianglesCount = header.numTris;
      material->vexts = new float[trianglesCount * 18];
      //float* uvs = new float[trianglesCount * 6];
      for (int k = 0; k < header.numTris; ++k)
      {
        Md2Triangle& triIdx = triangles[k];
        material->vexts[k*18+0] = startFrame[triIdx.vertexIndices[0]].x;
        material->vexts[k*18+1] = startFrame[triIdx.vertexIndices[0]].y;
        material->vexts[k*18+2] = startFrame[triIdx.vertexIndices[0]].z;
        material->vexts[k*18+3] = startFrame[triIdx.vertexIndices[1]].x;
        material->vexts[k*18+4] = startFrame[triIdx.vertexIndices[1]].y;
        material->vexts[k*18+5] = startFrame[triIdx.vertexIndices[1]].z;
        material->vexts[k*18+6] = startFrame[triIdx.vertexIndices[2]].x;
        material->vexts[k*18+7] = startFrame[triIdx.vertexIndices[2]].y;
        material->vexts[k*18+8] = startFrame[triIdx.vertexIndices[2]].z;
        material->vexts[k*18+9] = texCoords[triIdx.stIndices[0]].s;
        material->vexts[k*18+10] = texCoords[triIdx.stIndices[0]].t;
        material->vexts[k*18+11] = texCoords[triIdx.stIndices[1]].s;
        material->vexts[k*18+12] = texCoords[triIdx.stIndices[1]].s;
        material->vexts[k*18+13] = texCoords[triIdx.stIndices[2]].s;
        material->vexts[k*18+14] = texCoords[triIdx.stIndices[2]].s;
        float p1[3] = {material->vexts[k*15+0], material->vexts[k*15+1], material->vexts[k*15+2]};
        float p2[3] = {material->vexts[k*15+3], material->vexts[k*15+4], material->vexts[k*15+5]};
        float p3[3] = {material->vexts[k*15+6], material->vexts[k*15+7], material->vexts[k*15+8]};
        FmVec3 normal = CalculateNormal(p1, p2, p3);
        material->vexts[k*18+15] = normal.x;
        material->vexts[k*18+16] = normal.y;
        material->vexts[k*18+17] = normal.z;
      }
      unsigned short* indices = new unsigned short[header.numTris * 3];
      for (int k = 0; k < header.numTris * 3; ++k)
      {
        indices[k] = k;
      }
      int ib_size = header.numTris * 3 * sizeof(unsigned short);
      //material->pGPUIB = g_pRender->CreateStaticIB(indices, ib_size);
      //material->pSingleGPUVB = g_pRender->CreateStaticVB(material->vexts, (unsigned int)header.numTris * 18);
    }
  }

  delete [] frameBuffer;
  delete [] texCoords;
  delete [] triangles;
  delete [] vertexs;
  return ret;
}

model_t* load_model(const char* pszModelName, const char* type)
{
  Assert(pszModelName != NULL);
  if (strstr(pszModelName, ".md2"))
  {
    return load_md2_model(pszModelName);
  }
  else if (strstr(type, ".fbx"))
  {

  }
	return NULL;
}

void unload_model(model_t* pModel)
{
}
