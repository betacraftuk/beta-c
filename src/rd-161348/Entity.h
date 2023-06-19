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
    int removed;
    float heightOffset;
    float bbWidth;
    float bbHeight;
} Entity;

void entity_create(Entity* entity, Level* level);
void entity_tick(Entity* entity);
void entity_turn(Entity* entity, float xo, float yo);
void entity_resetPos(Entity* entity);
void entity_setPos(Entity* entity, float x, float y, float z);
void entity_setSize(Entity* entity, float w, float h);
void entity_move(Entity* entity, float xa, float ya, float za);
void entity_moveRelative(Entity* entity, float xa, float za, float speed);
int entity_isLit(Entity* entity);

#endif
