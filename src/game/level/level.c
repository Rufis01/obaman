#include "level.h"
#include "../camera/camera.h"
#include "../entity/entity.h"
#include "../entity/player.h"
#include "../entity/behaviour.h"
#include "../../renderer/model.h"
#include "../../renderer/renderer.h"
#include "../../filesystem/filesystem.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>

static inline char *i_G_InitCamera(G_State *state, char *data)
{
	G_Camera *cam = &state->camera;
	memcpy(cam, data, sizeof(G_Camera));

	///TODO: Code is buggy, we hardcode a value for now.
	cam->FOV=640;
	R_SetFov(cam->FOV);
	
	/*cam->rotation.vx = 0;
	cam->rotation.vy = 0;
	cam->rotation.vz = 0;*/

	printf("Camera:\n");
	printf("\tPosition:\n");
	printf("\t\tX: %d\n", cam->position.vx);
	printf("\t\tY: %d\n", cam->position.vy);
	printf("\t\tZ: %d\n", cam->position.vz);
	printf("\tRotation:\n");
	printf("\t\tX: %hd\n", cam->rotation.vx);
	printf("\t\tY: %hd\n", cam->rotation.vy);
	printf("\t\tZ: %hd\n", cam->rotation.vz);
	//printf("\tFOV: %d\n", (cam->FOV * 640) >> 12);

	return data + sizeof(G_Camera);
}
static inline char *i_G_LoadEntityData(const char *levelpath, G_Entity *ent, char *data)
{	
	ent->size = sizeof(G_Entity);
	memcpy(&ent->position, data, sizeof(VECTOR)); 
	data += sizeof(VECTOR);

	memcpy(&ent->rotation, data, sizeof(SVECTOR)); 
	data += sizeof(SVECTOR);

	memset(&ent->velocity, 0, sizeof(SVECTOR));

	const char *name = data;
	char *directory = FS_GetDirectoryPath(levelpath);
	char *fullpath = FS_GetFullPath(directory, name);

	ent->model = R_LoadModel(fullpath);

	printf("%s:\n", name);
	printf("\t\tPath: %s\n", fullpath);
	printf("\t\tPosition:\n");
	printf("\t\t\tX: %d\n", ent->position.vx);
	printf("\t\t\tY: %d\n", ent->position.vy);
	printf("\t\t\tZ: %d\n", ent->position.vz);
	printf("\t\tRotation:\n");
	printf("\t\t\tX: %hd\n", ent->rotation.vx);
	printf("\t\t\tY: %hd\n", ent->rotation.vy);
	printf("\t\t\tZ: %hd\n", ent->rotation.vz);

	free(directory);
	free(fullpath);

	return data + strlen(name) + 1;
}

static inline char *i_G_AddEntity(const char *levelpath, G_State *state, char *data)
{
	if(state->active_entities >= MAX_ENTITIES)
		return 0;

	G_Entity *ent = state->entities[state->active_entities++] = malloc(sizeof(G_Entity));

	if(!ent)
		return 0;
	
	return i_G_LoadEntityData(levelpath, ent, data);
}

static inline char *i_G_AddPlayer(const char *path, G_State *state, char *data)
{
	
	if(state->active_players >= MAX_PLAYERS ||
	   state->active_entities >= MAX_ENTITIES)
		return 0;

	G_Player *player = malloc(sizeof(G_Player));
	state->active_players++;

	if(!player)
		return 0;

	data = i_G_LoadEntityData(path, (G_Entity *)player, data);

	player->_ent.type = ENTITY_PLAYER;
	player->_ent.size = sizeof(G_Player);

	G_E_BehaviourHeader header =
	{
		.size = sizeof(G_E_BehaviourHeader),
		.behaviour = d_E_ChainTest
	};

	E_AddBehaviour((G_Entity **) &player, &header);
	header.behaviour = d_E_FreeCam;
	E_AddBehaviour((G_Entity **) &player, &header);

	state->entities[state->active_entities++] = (G_Entity *) player;
	printf("Player is at %p\n", player);

	return data;
}

G_State *G_LoadLevel(const char *levelname)
{
	int size;
	char *cpath = FS_GetCorrectPath(levelname);
	char *data = FS_LoadFileSync(cpath, &size);
	char *level = data;
	G_State *state;

	if(!data)
		return 0;
	
	if(!(state = malloc(sizeof(G_State))))
		return 0;
	
	memset(state, 0, sizeof(G_State));

	while(level < data + size)
	{
		char obj = *level++;
		switch(obj)
		{
			case 'c':
			{
				level = i_G_InitCamera(state, level);
			} break;
			case 'o':
			{
				level = i_G_AddEntity(cpath, state, level);
			} break;
			case 'p':
			{
				level = i_G_AddPlayer(cpath, state, level);
			} break;
			default:
			{
				printf("Yo, wtf? I don't know what this is! %#x\n", obj);
			}
		}
	}

	free(data);
	free(cpath);
	
	return state;
}
