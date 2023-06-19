#include "Chunk.h"

#include "../Util.h"
#include "../Textures.h"
#include "tile/Tile.h"
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

int chunk_updates = 0;
uint64_t chunk_totalTime = 0;
int chunk_totalUpdates = 0;

void chunk_create(Chunk* chunk, int x0, int y0, int z0, int x1, int y1, int z1) {
    chunk->x0 = x0;
    chunk->y0 = y0;
    chunk->z0 = z0;
    chunk->x1 = x1;
    chunk->y1 = y1;
    chunk->z1 = z1;
    chunk->x = (float)(x0 + x1) / 2.0F;
    chunk->y = (float)(y0 + y1) / 2.0F;
    chunk->z = (float)(z0 + z1) / 2.0F;
    chunk->dirty = 1;
    chunk->dirtiedTime = 0;
    aabb_create(&chunk->aabb, x0, y0, z0, x1, y1, z1);
    chunk->lists = glGenLists(2);
}

void chunk_rebuildLayer(Level* level, Chunk* chunk, int layer) {
    chunk->dirty = 0;
    chunk_updates++;
    uint64_t before = util_getTimeInNano();
    glNewList(chunk->lists + layer, GL_COMPILE);
    tesselator_init();
    int tiles = 0;

    for (int after = chunk->x0; after < chunk->x1; after++) {
        for (int y = chunk->y0; y < chunk->y1; y++) {
            for (int z = chunk->z0; z < chunk->z1; z++) {
                int tileId = level_getTile(level, after, y, z);
                if (tileId > 0) {
                    tile_render(&tile_tiles[tileId], level, layer, after, y, z);
                    tiles++;
                }
            }
        }
    }

    tesselator_flush();
    glEndList();
    uint64_t var9 = util_getTimeInNano();

    if (tiles > 0) {
        chunk_totalTime += var9 - before;
        chunk_totalUpdates++;
    }
}

void chunk_rebuild(Level* level, Chunk* chunk) {
    chunk_rebuildLayer(level, chunk, 0);
    chunk_rebuildLayer(level, chunk, 1);
}

void chunk_render(Chunk* chunk, int layer) {
    glCallList(chunk->lists + layer);
}

void chunk_setDirty(Chunk* chunk) {
    if (!chunk->dirty) {
        chunk->dirtiedTime = util_getTimeInMs();
    }

    chunk->dirty = 1;
}

float chunk_distanceToSqr(Chunk* chunk, Player* player) {
    float xd = player->entity.x - chunk->x;
    float yd = player->entity.y - chunk->y;
    float zd = player->entity.z - chunk->z;
    return xd * xd + yd * yd + zd * zd;
}
