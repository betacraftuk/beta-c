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
    float x;
    float y;
    float z;
	int dirty;
	int lists;
    uint64_t dirtiedTime;
} Chunk;

typedef struct ChunkArray {
    Chunk arr[1024];
    int size;
} ChunkArray;

typedef struct LevelRenderer {
    Chunk* chunks;
    int chunksSize;
    int xChunks;
	int yChunks;
	int zChunks;
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
    int unprocessed;
} Level;

void level_create(Level* level, int w, int h, int d);
AABBArray* level_getCubes(Level* level, AABB* aabb);
void level_save(Level* level);
float level_getBrightness(Level* level, int x, int y, int z);
int level_setTile(Level* level, int x, int y, int z, int type);
int level_isSolidTile(Level* level, int x, int y, int z);
int level_getTile(Level* level, int x, int y, int z);
int level_isLit(Level* level, int x, int y, int z);
void level_tick(Level* level);

#endif