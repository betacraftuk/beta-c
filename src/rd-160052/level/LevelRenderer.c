#include "LevelRenderer.h"

#include "tile/Tile.h"
#include "Chunk.h"
#include "Frustum.h"
#include "../Util.h"
#include "../Textures.h"
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

Player* _player = NULL;

void levelrenderer_create(LevelRenderer* levelRenderer, Level* level) {
    levelRenderer->xChunks = level->width / 16;
    levelRenderer->yChunks = level->depth / 16;
    levelRenderer->zChunks = level->height / 16;
    levelRenderer->chunks = malloc(sizeof(Chunk)
        * (levelRenderer->xChunks 
        * levelRenderer->yChunks 
        * levelRenderer->zChunks));
    levelRenderer->chunksSize = 0;

    for (int x = 0; x < levelRenderer->xChunks; x++) {
        for (int y = 0; y < levelRenderer->yChunks; y++) {
            for (int z = 0; z < levelRenderer->zChunks; z++) {
                int x0 = x * 16;
                int y0 = y * 16;
                int z0 = z * 16;
                int x1 = (x + 1) * 16;
                int y1 = (y + 1) * 16;
                int z1 = (z + 1) * 16;

                if (x1 > level->width) {
                    x1 = level->width;
                }

                if (y1 > level->depth) {
                    y1 = level->depth;
                }

                if (z1 > level->height) {
                    z1 = level->height;
                }

                chunk_create(&levelRenderer->chunks[(x + y * levelRenderer->xChunks) * levelRenderer->zChunks + z], x0, y0, z0, x1, y1, z1);
                levelRenderer->chunksSize++;
            }
        }
    }
}

void levelrenderer_getAllDirtyChunks(ChunkArray* dirty, LevelRenderer* renderer) {
    dirty->size = 0;

    for (int i = 0; i < renderer->chunksSize; ++i) {
        Chunk* chunk = &renderer->chunks[i];

        if (chunk->dirty) {
            dirty->arr[dirty->size] = chunk;
            dirty->size++;
        }
    }
}

void levelrenderer_render(Level* level, Player* player, int layer) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures_load("terrain.png", 9728));
    frustum_calculate();

    for (int i = 0; i < level->renderer.chunksSize; i++) {
        if (frustum_visibleAABB(&level->renderer.chunks[i].aabb)) {
            chunk_render(&level->renderer.chunks[i], layer);
        }
    }

    glDisable(GL_TEXTURE_2D);
}

int levelrenderer_dirtyChunkSorter(const void* a, const void* b) {
    Chunk* c0 = *(Chunk**)a;
    Chunk* c1 = *(Chunk**)b;

    int i0 = frustum_visibleAABB(&c0->aabb);
    int i1 = frustum_visibleAABB(&c1->aabb);

    if (i0 && !i1) {
        return -1;
    } 
    
    if (i1 && !i0) {
        return 1;
    }

    uint64_t now = util_getTimeInMs();
    int t0 = ((now - c0->dirtiedTime) / 2000L);
    int t1 = ((now - c1->dirtiedTime) / 2000L);

    return t0 < t1 ? -1 : (t0 > t1 ? 1 : (chunk_distanceToSqr(c0, _player) < chunk_distanceToSqr(c1, _player) ? -1 : 1));
}

void levelrenderer_updateDirtyChunks(Level* level, Player* player) {
    _player = player;
    ChunkArray dirty;
    levelrenderer_getAllDirtyChunks(&dirty, &level->renderer);

    if (dirty.size > 0) {
        qsort(dirty.arr, dirty.size, sizeof(Chunk*), levelrenderer_dirtyChunkSorter);

        for (int i = 0; i < 8 && i < dirty.size; i++) {
            chunk_rebuild(level, dirty.arr[i]);
        }
    }
}

void levelrenderer_renderPick(Level* level, Player* player) {
    float r = 3.0F;
    AABB box;
    aabb_grow(&box, &player->entity.bb, r, r, r);
    int x0 = box.x0;
    int x1 = box.x1 + 1.0F;
    int y0 = box.y0;
    int y1 = box.y1 + 1.0F;
    int z0 = box.z0;
    int z1 = box.z1 + 1.0F;
    glInitNames();

    for (int x = x0; x < x1; x++) {
        glPushName(x);

        for (int y = y0; y < y1; y++) {
            glPushName(y);

            for (int z = z0; z < z1; z++) {
                glPushName(z);
                Tile* tile = &tile_tiles[level_getTile(level, x, y, z)];

                if (!tile->isNull) {
                    glPushName(0);

                    for (int i = 0; i < 6; i++) {
                        glPushName(i);
                        tesselator_init();
                        tile_renderFaceNoTexture(tile, x, y, z, i);
                        tesselator_flush();
                        glPopName();
                    }

                    glPopName();
                }

                glPopName();
            }

            glPopName();
        }

        glPopName();
    }
}

void levelrenderer_renderHit(LevelRenderer* renderer, HitResult* h) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0F, 1.0F, 1.0F, (float)sin((double)util_getTimeInMs() / 100.0) * 0.2F + 0.4F);
    tesselator_init();
    tile_renderFaceNoTexture(&tile_rock, h->x, h->y, h->z, h->f);
    tesselator_flush();
    glDisable(GL_BLEND);
}

void levelrenderer_setDirty(LevelRenderer* renderer, int x0, int y0, int z0, int x1, int y1, int z1) {
    x0 /= 16;
    x1 /= 16;
    y0 /= 16;
    y1 /= 16;
    z0 /= 16;
    z1 /= 16;

    if (x0 < 0) {
        x0 = 0;
    }

    if (y0 < 0) {
        y0 = 0;
    }

    if (z0 < 0) {
        z0 = 0;
    }

    if (x1 >= renderer->xChunks) {
        x1 = renderer->xChunks - 1;
    }

    if (y1 >= renderer->yChunks) {
        y1 = renderer->yChunks - 1;
    }

    if (z1 >= renderer->zChunks) {
        z1 = renderer->zChunks - 1;
    }

    for (int x = x0; x <= x1; x++) {
        for (int y = y0; y <= y1; y++) {
            for (int z = z0; z <= z1; z++) {
                chunk_setDirty(&renderer->chunks[(x + y * renderer->xChunks) * renderer->zChunks + z]);
            }
        }
    }

}

void levelrenderer_tileChanged(LevelRenderer* renderer, int x, int y, int z) {
    levelrenderer_setDirty(renderer, x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void levelrenderer_lightColumnChanged(LevelRenderer* renderer, int x, int z, int y0, int y1) {
    levelrenderer_setDirty(renderer, x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
}

void levelrenderer_allChanged(Level* level) {
    levelrenderer_setDirty(&level->renderer, 0, 0, 0, level->width, level->depth, level->height);
}
