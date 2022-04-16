#include "game.h"
#include "level/level.h"
#include "../renderer/renderer.h"


void G_Start()
{
	G_State *state = G_LoadLevel("TEST.OSF");

	while(1)
	{
		R_DbgPrint("OBA-MAN DEMO\n");

		for(unsigned int i=0; i<state->active_entities; i++)
		{
			G_Entity *ent = state->entities[i];
			E_Update(state, ent);
		}

		for(unsigned int i=0; i<state->active_entities; i++)
		{
			G_Entity *ent = state->entities[i];
			E_Draw(state, ent);
		}

		R_DbgPrint("X=%d Y=%d Z=%d\n", 
			state->camera.position.vx>>12, 
			state->camera.position.vy>>12, 
			state->camera.position.vz>>12);
		R_DbgPrint("RX=%d RY=%d FOV=%d\n", 
			state->camera.rotation.vx, 
			state->camera.rotation.vy,
			state->camera.FOV);
		
		R_Finish();
	}
}
