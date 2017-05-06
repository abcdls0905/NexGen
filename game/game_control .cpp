
#include "game_control.h"
#include "../visual/i_terrain.h"
#include "global.h"
#include "util/macro_util.h"
#include "../visual/i_actor.h"
#include "../world/actor.h"
#include "../world/model.h"
#include "../visual/i_scene.h"
#include "../visual/i_camera.h"
#include "../game/camera/camera_control.h"
#include "game_util.h"

DECLARE_ENTITY(GameControl, 0, IEntity);

GameControl::GameControl(void)
{

}

GameControl::~GameControl(void)
{

}

bool GameControl::Init(const IVarList& args)
{
  CORE_ADD_EXECUTE(this);
  g_core->AddMsgProc(this, WM_KEYDOWN);
  g_core->AddMsgProc(this, WM_LBUTTONDOWN);
  g_core->AddMsgProc(this, WM_LBUTTONUP);
  g_core->AddMsgProc(this, WM_MBUTTONDOWN);
  g_core->AddMsgProc(this, WM_MBUTTONUP);
  g_core->AddMsgProc(this, WM_CHAR);
  g_core->AddMsgProc(this, WM_KEYUP);
  g_core->AddMsgProc(this, WM_MOUSEMOVE);

  g_core->AddMsgProc(this, FM_TOUCH_BEGIN);
  g_core->AddMsgProc(this, FM_TOUCH_END);
  g_core->AddMsgProc(this, FM_TOUCH_MOVE);
  g_core->AddMsgProc(this, FM_TOUCH_CANCELLED);
  return true;
}

bool GameControl::Shut()
{
  g_core->RemoveMsgProc(this, WM_KEYDOWN);
  g_core->RemoveMsgProc(this, WM_LBUTTONDOWN);
  g_core->RemoveMsgProc(this, WM_LBUTTONUP);
  g_core->RemoveMsgProc(this, WM_MBUTTONDOWN);
  g_core->RemoveMsgProc(this, WM_MBUTTONUP);
  g_core->RemoveMsgProc(this, WM_CHAR);
  g_core->RemoveMsgProc(this, WM_KEYUP);
  g_core->RemoveMsgProc(this, WM_MOUSEMOVE);

  g_core->RemoveMsgProc(this, FM_TOUCH_BEGIN);
  g_core->RemoveMsgProc(this, FM_TOUCH_END);
  g_core->RemoveMsgProc(this, FM_TOUCH_MOVE);
  g_core->RemoveMsgProc(this, FM_TOUCH_CANCELLED);
  return true;
}

float angle = 0;

void GameControl::Execute(float seconds)
{
}

bool GameControl::MsgProc(unsigned int msg, size_t param1, size_t param2, int& result)
{
  if (WM_MOUSEMOVE == msg)
  {
    g_mouse_pos_x = LOWORD(param2);
    g_mouse_pos_y = HIWORD(param2);
  }
  switch(msg)
  {
#ifdef _WIN32
  case WM_KEYDOWN:
    {
      return MsgKeyDown(param1 ,param2);
      break;
    }
  case WM_KEYUP:
    {
      break;
    }
  }
#endif
  return false;
}

Model* actor = NULL;
Model* actor1 = NULL;

bool GameControl::MsgKeyDown(size_t param1, size_t param2)
{
#ifdef _WIN32
  switch(param1)
  {
  case VK_F3:
    {
      break;
      //用来切换第三人称和自由视角
      CameraControl* pCameraControl = (CameraControl*)g_core->LookupEntity("CameraControl");
      if (pCameraControl != NULL)
      {
        int iCameraType = pCameraControl->GetActiveType();
        if (iCameraType != MAX_CAMERA-1)
        {
          pCameraControl->SetActive(iCameraType+1);
        }
        else
        {
          pCameraControl->SetActive(CAMERA_NONE+1);
        }
      }
      break;
    }
  case 'N':
    {
      if (actor)
      {
        break;
      }
      IModel* iActor = (IModel*)g_pCore->CreateEntity("Model");
      actor = (Model*) iActor;
      actor->SetContext(g_terrain->GetContext());
      actor->Load("md2\\拿刀的古代士兵\\tris.md2", "");
      actor->SetPosition(0, 0, 0);
      //actor->SetAngle(0, 0, FM_PI/2);
      //actor->SetCastShadow(true);
      actor->m_pModelPlayer->SetReceiveShadow(true);
      g_terrain->AddVisualRole("test_player", actor->GetID());
      break;
    }
  case 'C':
    {
      if (actor1)
        break;
      IModel* iActor = (IModel*)g_pCore->CreateEntity("Actor");
      actor1 = (Model*) iActor;
      actor1->SetContext(g_terrain->GetContext());
      actor1->Load("md2\\拿刀的古代士兵\\tris.md2", "");
      actor1->SetPosition(3, 0, 0);
      actor1->SetAngle(0, 0, FM_PI/2);
      actor1->SetCastShadow(true);
      //actor1->m_pModelPlayer->SetReceiveShadow(true);
      g_terrain->AddVisualRole("test_player1", actor1->GetID());
      break;
    }
  case 'M':
    {
      g_render->GetRenderDrawOp()->DrawTest();
      ICamera* pCamera = (ICamera*)g_core->GetEntity(g_scene->GetCameraID());
      pCamera->SetPosition(2.497921, 27.894909, -10.118525);
      pCamera->SetAngle(-0.116355, -1.230457, 0.000000);
      break;
    }
  case 'B':
    {
      g_scene->SetEnableDynamicShadow(true);
      break;
    }
  case 'V':
    {
      break;
    }
  case 'L':
    {
      char mbzTemp[256];
      sprintf(mbzTemp,"%s%s%s/", g_core->GetResourcePath(), "map/ter/", "digong_lingmu_1");
      terrain_load(mbzTemp);
      break;
    }
  case VK_LEFT:
    {
      if (actor)
      {
        FmVec3 angle = actor->GetAngle();
        actor->SetAngle(angle.x + 0.3f, angle.y, angle.z);
      }
      break;
    }
  case VK_RIGHT:
    {
      if (actor)
      {
        FmVec3 angle = actor->GetAngle();
        actor->SetAngle(angle.x - 0.3f, angle.y, angle.z);
      }
      break;
    }
  case VK_UP:
    {
      if (actor)
      {
        FmVec3 angle = actor->GetAngle();
        actor->SetAngle(angle.x, angle.y - 0.3f, angle.z);
      }
      break;
    }
  case VK_DOWN:
    {
      if (actor)
      {
        FmVec3 angle = actor->GetAngle();
        actor->SetAngle(angle.x, angle.y + 0.3f, angle.z);
      }
      break;
    }
  case 'O':
    {
      IRenderContext* pContent = g_render->GetContext();
      g_render->SaveTextureToFile(pContent->GetDynamicShadowRT(), "test.png");
    }
  }
#endif
  return false;
}
