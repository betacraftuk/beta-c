#ifndef PLAYER_H
#define PLAYER_H

#include "level/Level.h"
#include "phys/AABB.h"
#include <SDL2/SDL.h>

typedef struct Player {
    Level* level;
    float xo;
	float yo;
	float zo;
	float x;
	float y;
	float z;
	float xd;
	float yd;
	float zd;
	float yRot;
	float xRot;
    AABB bb;
    int onGround;
	struct {
		int isW;
		int isA;
		int isS;
		int isD;
		int isSpace;
	} keys;
} Player;

void player_create(Player* player, Level* level);
void tick(Player* player);
void turnPlayer(Player* player, float xo, float yo);
void resetPos(Player* player);

#endif