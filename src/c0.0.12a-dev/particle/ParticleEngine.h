#ifndef PARTICLEENGINE_H
#define PARTICLEENGINE_H

#include "../particle/Particle.h"
#include "../Util.h"
#include "../Player.h"

typedef struct ParticleEngine {
    Level* level;
    LinkedList* particles;
} ParticleEngine;

void particleengine_create(ParticleEngine* pe, Level* level);
void particleengine_tick(ParticleEngine* pe);
void particleengine_render(ParticleEngine* pe, Player* player, float a, int layer);

#endif
