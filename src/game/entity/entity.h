#ifndef ENTITY_H
#define ENTITY_H

#include "psxgte.h"
#include "../../renderer/model.h"

struct _State;

typedef enum
{
	NONE,
	PLAYER,
	MAX_TYPES
} G_EntityType;

typedef struct _Entity
{
	VECTOR position;
	SVECTOR rotation;
	SVECTOR velocity;
	R_Model *model;
	G_EntityType type;
	void (*update)(struct _State *state, struct _Entity* self);
} G_Entity;

#endif
