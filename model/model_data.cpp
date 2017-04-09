

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

void WriteVertex(FmVec3* vertexs, int count)
{
  FILE* fp = fopen("vertex.txt", "wt");
  for (int i = 0; i < count; i++)
  {
    float data[3] = {vertexs[i].x, vertexs[i].y, vertexs[i].z};
    fwrite(&vertexs[i].x, sizeof(float), 1, fp);
    fwrite(&vertexs[i].y, sizeof(float), 1, fp);
    fwrite(&vertexs[i].z, sizeof(float), 1, fp);
    fwrite("\n", sizeof("\n"), 1, fp);
  }
  fclose(fp);
}

model_t* load_md2_model(const char* pszModelName, char* tex)
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
  unsigned char * frameBuffer = new unsigned char[header.framesize];
  for(int i=0; i<header.numFrames; ++i)
  {
    fseek(pFile, header.offsetFrames+i*header.framesize, SEEK_SET);
    fread(frameBuffer, header.framesize, 1, pFile);
    Md2Frame * framePtr=(Md2Frame *) frameBuffer;
    for(int j=0; j<header.numXYZ; ++j)
    {
      vertexs[i*numVertices+j].x= float(framePtr->scale[0] * framePtr->vertices[j].position[0] + framePtr->translation[0]);
      vertexs[i*numVertices+j].z= float(framePtr->scale[2] * framePtr->vertices[j].position[2] + framePtr->translation[2]);
      vertexs[i*numVertices+j].y=-float(framePtr->scale[1] * framePtr->vertices[j].position[1] + framePtr->translation[1]);
      vertexs[i*numVertices+j]/=10;
    }
  }

  //load textures
  Md2TexCoord* texCoords = new Md2TexCoord[header.numST];
  fseek(pFile, header.offsetST, SEEK_SET);
  fread(texCoords, sizeof(Md2TexCoord), header.numST, pFile);

  //load triangles
  int numTriangles=header.numTris;
  Md2Triangle* triangles=new Md2Triangle[numTriangles];
  fseek(pFile, header.offsetTris, SEEK_SET);
  fread(triangles, sizeof(Md2Triangle), numTriangles, pFile);

  //build VertexBuffer and IndexBuffer

  if (ret->pszTexBase == NULL)
  {
    // 获得模型所在的文件路径
    const char* path_end = pszModelName;
    const char* s = strrchr(pszModelName, '\\');
    if (s)
    {
      path_end = s + 1;
    }
    else
    {
      s = strrchr(pszModelName, '/');

      if (s)
      {
        path_end = s + 1;
      }
    }
    size_t size = path_end - pszModelName;
    ret->pszTexBase = (char*)CORE_ALLOC(size + 1);
    memcpy(ret->pszTexBase, pszModelName, size);
    ret->pszTexBase[size] = 0;
  }
  ret->nRootNodeCount = 1;
  ret->RootNodes = (model_node_t*)CORE_ALLOC(
    sizeof(model_node_t) * ret->nRootNodeCount);
  memset(ret->RootNodes, 0, sizeof(model_node_t) * ret->nRootNodeCount);

  for (int i = 0; i < ret->nRootNodeCount; ++i)
  {
    model_node_t* root = &ret->RootNodes[i];
    root->nModelInfo= MODEL_POSITION_INFO + MODEL_NORMAL_INFO;
    root->nMaterialCount = 1;
    root->nChildNodeCount = 0;
    root->Materials = new node_material_t[root->nMaterialCount];
    memset(root->Materials, 0, sizeof(node_material_t) * root->nMaterialCount);
    for (int j = 0; j < root->nMaterialCount; ++j)
    {
      node_material_t* material = &root->Materials[j];
      material->nMaterialInfo = MATERIAL_DIFFUSE_MAP_INFO;
      init_material_value(&material->MatInfo);
      {
        //设置贴图数据
        char* texFile = "evil.jpg";
        material->MatInfo.DiffuseMap.pFileName = texFile;
        material->MatInfo.DiffuseMap.nFileNameLen = strlen(texFile);
      }
      //关键构造

      //只取第一帧静态数据
      //build vb
      FmVec3* startFrame = &vertexs[0];
      int trianglesCount = header.numTris;
      int vertexContent = 8;
      int count = vertexContent*3;
      material->nSingleVertexSize = vertexContent*sizeof(float);
      material->SingleVB.nStride = material->nSingleVertexSize;
      material->SingleVB.nCount = trianglesCount * 3;
      material->nVertexCount = trianglesCount * 3;
      create_vertex_data(&material->SingleVB, material->SingleVB.nStride, material->nVertexCount);
      for (int k = 0; k < header.numTris; ++k)
      {
        Md2Triangle& triIdx = triangles[k];
        float v1x = startFrame[triIdx.vertexIndices[0]].x;
        float v1y = startFrame[triIdx.vertexIndices[0]].y;
        float v1z = startFrame[triIdx.vertexIndices[0]].z;
        float v2x = startFrame[triIdx.vertexIndices[1]].x;
        float v2y = startFrame[triIdx.vertexIndices[1]].y;
        float v2z = startFrame[triIdx.vertexIndices[1]].z;
        float v3x = startFrame[triIdx.vertexIndices[2]].x;
        float v3y = startFrame[triIdx.vertexIndices[2]].y;
        float v3z = startFrame[triIdx.vertexIndices[2]].z;

        float p1[3] = {v1x, v1y, v1z};
        float p2[3] = {v2x, v2y, v2z};
        float p3[3] = {v3x, v3y, v3z};

        FmVec3 normal = CalculateNormal(p1, p2, p3);

        float n1x = normal.x;
        float n1y = normal.y;
        float n1z = normal.z;
        float n2x = normal.x;
        float n2y = normal.y;
        float n2z = normal.z;
        float n3x = normal.x;
        float n3y = normal.y;
        float n3z = normal.z;

        float uv1s = (float)texCoords[triIdx.stIndices[0]].s/header.skinwidth;
        float uv1t = (float)texCoords[triIdx.stIndices[0]].t/header.skinheight;
        float uv2s = (float)texCoords[triIdx.stIndices[1]].s/header.skinwidth;
        float uv2t = (float)texCoords[triIdx.stIndices[1]].t/header.skinheight;
        float uv3s = (float)texCoords[triIdx.stIndices[2]].s/header.skinwidth;
        float uv3t = (float)texCoords[triIdx.stIndices[2]].t/header.skinheight;

        material->SingleVB.pVertices[k*count+0] = v1x;
        material->SingleVB.pVertices[k*count+1] = v1y;
        material->SingleVB.pVertices[k*count+2] = v1z;
        material->SingleVB.pVertices[k*count+3] = n1x;
        material->SingleVB.pVertices[k*count+4] = n1y;
        material->SingleVB.pVertices[k*count+5] = n1z;
        material->SingleVB.pVertices[k*count+6] = uv1s;
        material->SingleVB.pVertices[k*count+7] = uv1t;
        material->SingleVB.pVertices[k*count+8] = v2x;
        material->SingleVB.pVertices[k*count+9] = v2y;
        material->SingleVB.pVertices[k*count+10] = v2z;
        material->SingleVB.pVertices[k*count+11] = n2x;
        material->SingleVB.pVertices[k*count+12] = n2y;
        material->SingleVB.pVertices[k*count+13] = n2z;
        material->SingleVB.pVertices[k*count+14] = uv2s;
        material->SingleVB.pVertices[k*count+15] = uv2t;
        material->SingleVB.pVertices[k*count+16] = v3x;
        material->SingleVB.pVertices[k*count+17] = v3y;
        material->SingleVB.pVertices[k*count+18] = v3z;
        material->SingleVB.pVertices[k*count+19] = n3x;
        material->SingleVB.pVertices[k*count+20] = n3y;
        material->SingleVB.pVertices[k*count+21] = n3z;
        material->SingleVB.pVertices[k*count+22] = uv3s;
        material->SingleVB.pVertices[k*count+23] = uv3t;

      }
      material->nIndicesCount = header.numTris * 3;
      material->indices = new unsigned short[header.numTris * 3];
      for (int k = 0; k < header.numTris * 3; ++k)
      {
        material->indices[k] = k;
      }
    }
  }

  delete [] frameBuffer;
  delete [] texCoords;
  delete [] triangles;
  delete [] vertexs;
  return ret;
}

model_t* load_model(const char* pszModelName, char* tex)
{
  Assert(pszModelName != NULL);
  if (strstr(pszModelName, ".md2"))
  {
    return load_md2_model(pszModelName, tex);
  }
  else if (strstr(pszModelName, ".fbx"))
  {

  }
	return NULL;
}

void unload_model(model_t* pModel)
{
}
