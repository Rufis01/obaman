#ifndef GAME_H
#define GAME_H

#include "camera/camera.h"
#include "entity/entity.h"

#define MAX_ENTITIES 5

typedef struct
{
	G_Camera camera;
	G_Entity entities[MAX_ENTITIES];
	G_Entity *player;
	unsigned int active_entities;
} G_State;

void G_Start();

#endif