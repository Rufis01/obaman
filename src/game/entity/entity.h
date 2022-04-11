#ifndef ENTITY_H
#define ENTITY_H

#include "psxgte.h"
#include "../../renderer/model.h"

typedef enum
{
	NONE,
	PLAYER,
	MAX_TYPES
} G_EntityType;

typedef struct
{
	VECTOR position;
	SVECTOR rotation;
	SVECTOR velocity;
	R_Model *model;
	G_EntityType type;
	void (*update)(G_State *state, G_Entity * self);
} G_Entity;

#endif