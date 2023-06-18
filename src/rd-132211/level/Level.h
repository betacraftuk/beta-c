#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

#include "Tesselator.h"
#include "../phys/AABB.h"

typedef struct Chunk {
    AABB aabb;
    int x0;
	int y0;
	int z0;
	int x1;
	int y1;
	int z1;
	int dirty;
	int lists;
} Chunk;

typedef struct LevelRenderer {
    Chunk* chunks;
    int chunksSize;
    int xChunks;
	int yChunks;
	int zChunks;
    Tesselator tesselator;
} LevelRenderer;

typedef struct Level {
    int width;
    int height;
    int depth;
    uint8_t* blocks;
    int blocksSize;
    int* lightDepths;
    int levelListenersSize;
    LevelRenderer renderer;
} Level;

void level_create(Level* level, int w, int h, int d);
int level_isTile(Level* level, int x, int y, int z);
AABBArray* level_getCubes(Level* level, AABB* aabb);
void level_save(Level* level);
float level_getBrightness(Level* level, int x, int y, int z);
void level_setTile(Level* level, int x, int y, int z, int type);

#endif