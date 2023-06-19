#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "ZombieModel.h"
#include "../Entity.h"

typedef struct Zombie {
    Entity entity;
    ZombieModel zombieModel;
	float rot;
	float timeOffs;
	float speed;
	float rotA;
} Zombie;

void zombie_create(Zombie* zombie, Level* level, float x, float y, float z);
void zombie_tick(Zombie* zombie);
void zombie_render(Zombie* zombie, float a);

#endif
