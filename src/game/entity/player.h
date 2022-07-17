#ifndef PLAYER_H
#define PLAYER_H

#include "../game.h"
#include "entity.h"
#include "behaviour.h"

typedef struct _Player		//Is this really useful (in this case)???
{
	G_Entity _ent;		// The player can be safely treated as an entity since a pointer to a struct always points to its first member.
} G_Player;

bool d_E_FreeCam(struct _State *state, struct _Entity *self, struct _E_BehaviourHeader* header);
bool d_E_ChainTest(struct _State *state, struct _Entity *self, struct _E_BehaviourHeader* header);

#endif
