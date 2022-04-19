#ifndef ENTITY_H
#define ENTITY_H

#include "psxgte.h"
#include "../game.h"
#include "../../renderer/model.h"


struct _E_UpdateNode;

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
	G_EntityType type;
	R_Model *model;
	struct _E_UpdateNode *update;
} G_Entity;

typedef bool(*E_Behaviour)(G_State *state, G_Entity *self);

void E_Update(G_State *state, G_Entity *self);
void E_Draw(G_State *state, G_Entity *self);

bool E_AddBehaviour(G_Entity *ent, E_Behaviour behaviour);

#endif
