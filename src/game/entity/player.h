#ifndef PLAYER_H
#define PLAYER_H

#include "../game.h"
#include "entity.h"

typedef struct _Player
{
	G_Entity _ent;		// The player can be safely treated as an entity since a pointer to a struct always points to its first member.
	//Other fields (function pointers?)
} G_Player;

void d_E_FreeCam(struct _State *state, struct _Entity *self);

#endif
