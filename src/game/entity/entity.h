#ifndef ENTITY_H
#define ENTITY_H

#include "psxgte.h"
#include "../game.h"
#include "../../renderer/model.h"


typedef enum
{
	ENTITY_NONE,
	ENTITY_PLAYER,
	ENTITY_ENEMY,
	ENTITY_OBJECT,
	ENTITY_MAX_TYPES
} G_EntityType;

typedef struct _Entity
{
	unsigned short size;
	VECTOR position;
	SVECTOR rotation;
	VECTOR velocity;
	G_EntityType type;
	R_Model *model;
	unsigned short nBehaviour;
} G_Entity;

void E_Update(G_State *state, G_Entity *self);
void E_Draw(G_State *state, G_Entity *self);

#endif
