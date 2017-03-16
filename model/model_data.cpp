

#include "model_data.h"

#include "../visual/i_texture.h"
#include "../visual/vis_utils.h"
#include "../public/inlines.h"
#include "../public/core_file.h"
#include "../public/core_log.h"


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
      //关键构造
      material->pGPUIB;
      material->pSingleGPUVB;
      material->MatInfo;

      //只取第一帧静态数据
      //build ib
      FmVec3* startFrame = &vertexs[0];
      int trianglesCount = header.numTris;
      float* vexts = new float[trianglesCount * 15];
      //float* uvs = new float[trianglesCount * 6];
      for (int k = 0; k < header.numTris; ++k)
      {
        Md2Triangle& triIdx = triangles[k];
        vexts[k*15+0] = startFrame[triIdx.vertexIndices[0]].x;
        vexts[k*15+1] = startFrame[triIdx.vertexIndices[0]].y;
        vexts[k*15+2] = startFrame[triIdx.vertexIndices[0]].z;
        vexts[k*15+3] = startFrame[triIdx.vertexIndices[1]].x;
        vexts[k*15+4] = startFrame[triIdx.vertexIndices[1]].y;
        vexts[k*15+5] = startFrame[triIdx.vertexIndices[1]].z;
        vexts[k*15+6] = startFrame[triIdx.vertexIndices[2]].x;
        vexts[k*15+7] = startFrame[triIdx.vertexIndices[2]].y;
        vexts[k*15+8] = startFrame[triIdx.vertexIndices[2]].z;
        vexts[k*15+9] = texCoords[triIdx.stIndices[0]].s;
        vexts[k*15+10] = texCoords[triIdx.stIndices[0]].t;
        vexts[k*15+11] = texCoords[triIdx.stIndices[1]].s;
        vexts[k*15+12] = texCoords[triIdx.stIndices[1]].s;
        vexts[k*15+13] = texCoords[triIdx.stIndices[2]].s;
        vexts[k*15+14] = texCoords[triIdx.stIndices[2]].s;
      }
      //material->pGPUIB = g_pRender->CreateStaticIB(pMat->pLODIB[pMat->nCurLODLevel], ib_size);

      //build vb

      //setmatnfo
      delete [] vertexs;
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
