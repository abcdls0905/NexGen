
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

class ICore;
class IWorld;
class GameControl;
class IScene;
class IRender;
class ITerrain;

extern ICore* g_core;
extern IWorld* g_world;
extern GameControl* g_gamecontrol;
extern IScene* g_scene;
extern IRender* g_render;
extern ITerrain* g_terrain;

extern int g_mouse_pos_x;
extern int g_mouse_pos_y;

#endif