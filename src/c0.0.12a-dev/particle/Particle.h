#ifndef PARTICLE_H
#define PARTICLE_H

#include "../Entity.h"

typedef struct Particle {
    Entity entity;
    float xd;
	float yd;
	float zd;
	int tex;
	float uo;
	float vo;
    int age;
    int lifetime;
    float size;
} Particle;

void particle_create(Particle* particle, Level* level, float x, float y, float z, float xa, float ya, float za, int tex);
void particle_tick(Particle* particle);
void particle_render(Particle* particle, float a, float xa, float ya, float za, float xa2, float za2);

#endif
