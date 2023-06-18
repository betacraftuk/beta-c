#ifndef CHUNK_H
#define CHUNK_H

#include "../Player.h"

extern int chunk_updates;
extern uint64_t chunk_totalTime;
extern int chunk_totalUpdates;

void chunk_create(Chunk* chunk, int x0, int y0, int z0, int x1, int y1, int z1);
void chunk_render(Chunk* chunk, int layer);
void chunk_rebuildLayer(Level* level, Chunk* chunk, int layer);
void chunk_rebuild(Level* level, Chunk* chunk);
void chunk_setDirty(Chunk* chunk);
float chunk_distanceToSqr(Chunk* chunk, Player* player);

#endif
