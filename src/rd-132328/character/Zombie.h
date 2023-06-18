#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Cube.h"
#include "../level/Level.h"
#include "../Entity.h"

typedef struct Zombie {
    Entity entity;
	Cube head;
	Cube body;
	Cube arm0;
	Cube arm1;
	Cube leg0;
	Cube leg1;
	float rot;
	float timeOffs;
	float speed;
	float rotA;
} Zombie;

void zombie_create(Zombie* zombie, Level* level, float x, float y, float z);
void zombie_tick(Zombie* zombie);
void zombie_render(Zombie* zombie, float a);

#endif
