#include "entity.h"
#include "../game.h"
#include "../../renderer/renderer.h"

#include <stdlib.h>

typedef struct _E_UpdateNode
{
	struct _E_UpdateNode* next;
	bool (*update)(struct _State *state, struct _Entity *self);
}i_E_UpdateNode;

void E_AddBehaviour(G_Entity *ent, E_Behaviour behaviour)
{
	i_E_UpdateNode **node = &ent->update;
	while(*node)
		*node = (*node)->next;

	*node = malloc(sizeof(i_E_UpdateNode));
	if(!*node)
		return;

	(*node)->next = 0;
	(*node)->update = behaviour;
}

void E_Update(G_State *state, G_Entity *self)
{
	i_E_UpdateNode *node = self->update;
	while(node)
	{
		if(!(node->update(state, self)))
			break;
		node = node->next;
	}
}

void E_Draw(G_State *state, G_Entity *self)
{
	R_DrawEntity(&state->camera, self);
}
