#ifndef RENDERER_H
#define RENDERER_H

#include "../game/entity/entity.h"
#include "../game/camera/camera.h"

void R_Init();
void R_DrawEntity(G_Camera *cam, G_Entity *ent);
void R_Finish();

void R_SetFov(int tan);

#include <psxgpu.h>
#define R_DbgPrint(...) FntPrint(-1, __VA_ARGS__);

#endif
