#include "level.h"
#include "../camera/camera.h"
#include "../entity/entity.h"
#include "../entity/player.h"
#include "../../renderer/model.h"
#include "../../renderer/renderer.h"
#include "../../filesystem/filesystem.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>

static inline char *i_G_InitCamera(G_State* state, char *data)
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

static inline char *i_G_AddEntity(const char* path, G_State* state, char *data)
{
	char modpath[255];
	int length;
	const char *lastdir = strrchr(path, '/');
	if(!lastdir) lastdir = strrchr(path, '\\');
	if(!lastdir)
		length = strlen(path);
	else
		length = (int)(lastdir-path);

	//G_Entity *ent = state->entities + state->active_entities;
	///FIX: This is broken. The entity array inside the gamestate has been changed to an entity pointer array!
	G_Entity *ent = 0;
	assert(0);

	memcpy(&ent->position, data, sizeof(VECTOR)); 
	data += sizeof(VECTOR);

	memcpy(&ent->rotation, data, sizeof(SVECTOR)); 
	data += sizeof(SVECTOR);

	memset(&ent->velocity, 0, sizeof(SVECTOR));

	const char *name = data;
	if(length)
		snprintf(modpath, 255, "%.*s\\%s", length, name, path);	//Path and name are in reverse order for some reason...
	else
		snprintf(modpath, 255, "\\%s", name);
	ent->model = R_LoadModel(modpath);
	state->active_entities++;

	printf("%s:\n", name);
	printf("\t\tPath: %s\n", modpath);
	printf("\t\tPosition:\n");
	printf("\t\t\tX: %d\n", ent->position.vx);
	printf("\t\t\tY: %d\n", ent->position.vy);
	printf("\t\t\tZ: %d\n", ent->position.vz);
	printf("\t\tRotation:\n");
	printf("\t\t\tX: %hd\n", ent->rotation.vx);
	printf("\t\t\tY: %hd\n", ent->rotation.vy);
	printf("\t\t\tZ: %hd\n", ent->rotation.vz);
	
	return data + strlen(name) + 1;
}

G_State* G_LoadLevel(const char *path)
{
	int size;
	char *data = FS_LoadFileSync(path, &size);
	char *level = data;
	G_State* state;

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
				level = i_G_AddEntity(path, state, level);
			} break;
			case 'p':
			{
				//level = i_G_AddPlayer(path, state, level);
			} break;
			default:
			{
				printf("Yo, wtf? I don't know what this is! %#x\n", obj);
			}
		}
	}

	free(data);
	
	return state;
}
