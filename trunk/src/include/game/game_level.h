#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "base/nebu_vector.h"

typedef struct game_spawnpoint {
	vec2 v;
	int dir;
} game_spawnpoint;

typedef struct game_level {
	int nBoundaries;
	segment2 *boundaries;
	int nSpawnPoints;
	int spawnIsRelative;
	game_spawnpoint *spawnPoints;
	box2 boundingBox;
	float scale_factor;
} game_level;

void game_FreeLevel(game_level *l);
game_level* game_CreateLevel();
void game_ScaleLevel(game_level *l, float fSize);
void game_UnloadLevel(void);
int game_LoadLevel(void);

enum {
	GAME_SUCCESS = 0,
	GAME_ERROR_LEVEL_ALREADYLOADED = 1,
	GAME_ERROR_LEVEL_NOTLOADED,
};

#endif
