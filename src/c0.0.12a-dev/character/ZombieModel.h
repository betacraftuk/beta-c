#ifndef ZOMBIEMODEL_H
#define ZOMBIEMODEL_H

#include "Cube.h"

typedef struct ZombieModel {
	Cube head;
	Cube body;
	Cube arm0;
	Cube arm1;
	Cube leg0;
	Cube leg1;
} ZombieModel;

void zombiemodel_create(ZombieModel* zombieModel);
void zombiemodel_render(ZombieModel* zombieModel, float time);

#endif
