#ifndef PLAYER_H
#define PLAYER_H

#include "level/Level.h"
#include "Entity.h"
#include <SDL2/SDL.h>

typedef struct Player {
    Entity entity;
    struct {
		int isW;
		int isA;
		int isS;
		int isD;
		int isSpace;
	} keys;
} Player;

void player_create(Player* player, Level* level);
void player_tick(Player* player);

#endif