#ifndef ENTITY_H
#define ENTITY_H

#include "level/Level.h"
#include "phys/AABB.h"
#include <SDL2/SDL.h>

typedef struct Entity {
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
    float heightOffset;
} Entity;

void entity_create(Entity* entity, Level* level);
void entity_tick(Entity* entity);
void entity_turn(Entity* entity, float xo, float yo);
void resetPos(Entity* entity);
void entity_move(Entity* entity, float xa, float ya, float za);
void moveRelative(Entity* entity, float xa, float za, float speed);

#endif
