#ifndef CHUNK_H
#define CHUNK_H

#include "Level.h"
#include "Tesselator.h"

extern Tesselator chunk_t;
extern int chunk_rebuiltThisFrame;
extern int chunk_updates;

void chunk_create(Chunk* chunk, int x0, int y0, int z0, int x1, int y1, int z1);
void chunk_render(Level* level, Chunk* chunk, int layer);

#endif
