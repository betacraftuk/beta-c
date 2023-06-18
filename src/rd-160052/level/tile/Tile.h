#ifndef TILE_H
#define TILE_H

#include "../../particle/ParticleEngine.h"

typedef struct Tile {
    int tex;
    int id;
    int isNull;
} Tile;

extern Tile tile_tiles[256];
extern Tile tile_rock;
extern Tile tile_grass;
extern Tile tile_dirt;
extern Tile tile_stoneBrick;
extern Tile tile_wood;

void tile_create(Tile* tile, int id, int tex, int isnull);
void tile_render(Tile* tile, Level* level, int layer, int x, int y, int z);
void tile_renderFaceNoTexture(Tile* tile, int x, int y, int z, int face);
void tile_getAABB(AABB* source, int x, int y, int z);
void tile_tick(Tile* tile, Level* level, int x, int y, int z);
void tile_destroy(Tile* tile, Level* level, int x, int y, int z, ParticleEngine* particleEngine);
void tile_init();

#endif
