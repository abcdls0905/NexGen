
#include "../public/module.h"
#include "terrain.h"
#include "../visual/i_vis_base.h"
#include "../visual/i_texture.h"
#include "../public/var_list.h"
#include "../public/core_log.h"
#include "../visual/i_render.h"
#include "../visual/i_context.h"

#include "../model/model_data.h"

// 初始化材质数据
void init_material_value(material_info_t* pMatInfo);

void Terrain::GenerateFractalTerrain()
{
  if (!m_pFractal)
    return;
  float *data = m_pFractal->GetData();
  int size = m_pFractal->GetSize();
  model_t* ret = (model_t*)CORE_ALLOC(sizeof(model_t));
  memset(ret, 0, sizeof(model_t));

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
      material->pNode = root;
      init_material_value(&material->MatInfo);
      {
        //设置贴图数据
        char* texFile = "evil.jpg";
        material->MatInfo.DiffuseMap.pFileName = texFile;
        material->MatInfo.DiffuseMap.nFileNameLen = strlen(texFile);
      }
    }
  }
}
