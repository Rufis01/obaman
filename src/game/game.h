#ifndef GAME_H
#define GAME_H

#include "camera/camera.h"

#define MAX_ENTITIES 5
#define MAX_PLAYERS 1

struct _Entity;
struct _Player;

typedef struct _State
{
	G_Camera camera;
	struct _Entity *entities[MAX_ENTITIES];
	struct _Player *players[MAX_PLAYERS];
	unsigned int active_entities;
	unsigned int active_players;
} G_State;

void G_Start(void);

#endif
