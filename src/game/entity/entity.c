#include "entity.h"
#include "behaviour.h"
#include "../game.h"
#include "../../renderer/renderer.h"

#include <stdlib.h>

void E_Update(G_State *state, G_Entity *self)
{
	G_E_BehaviourHeader *header = (G_E_BehaviourHeader *) (((char *)(self)) + sizeof(G_Entity));

	for(int i=0; i<self->nBehaviour; i++)
	{
		printf("Behaviour %d at %p size %d FUN: %p\n", i, header, header->size, header->behaviour);
		if(!(header->behaviour(state, self, header)))
			break;
		header = (G_E_BehaviourHeader *) (((char *)(header)) + header->size); 
	}
}

void E_Draw(G_State *state, G_Entity *self)
{
	R_DrawEntity(&state->camera, self);
}
