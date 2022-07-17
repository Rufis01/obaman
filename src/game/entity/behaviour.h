#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "entity.h"

struct _E_BehaviourHeader;

typedef bool(*E_Behaviour)(G_State *state, G_Entity *self, struct _E_BehaviourHeader* header);

typedef struct _E_BehaviourHeader
{
	unsigned short size;	//sizeof(G_E_BehaviourHeader) + data aka &this + this.size = &next
	unsigned short id;
	char *name;
	E_Behaviour behaviour;
} G_E_BehaviourHeader;

bool E_AddBehaviour(G_Entity **ent, G_E_BehaviourHeader *behaviour);
unsigned short E_BehaviourGetHash(const char* name);

#endif