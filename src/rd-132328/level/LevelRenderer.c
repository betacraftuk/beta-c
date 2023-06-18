#include "LevelRenderer.h"

#include "Tile.h"
#include "Chunk.h"
#include "Frustum.h"
#include "../Util.h"
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void levelrenderer_create(LevelRenderer* levelRenderer, Level* level) {
    levelRenderer->xChunks = level->width / 16;
    levelRenderer->yChunks = level->depth / 16;
    levelRenderer->zChunks = level->height / 16;
    levelRenderer->chunks = malloc(sizeof(Chunk)
        * (levelRenderer->xChunks 
        * levelRenderer->yChunks 
        * levelRenderer->zChunks));
    levelRenderer->chunksSize = 0;
    tesselator_create(&levelRenderer->tesselator);

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

void levelrenderer_render(Level* level, Player* player, int layer) {
    chunk_rebuiltThisFrame = 0;
    frustum_calculate();

    for (int i = 0; i < level->renderer.chunksSize; i++) {
        if (frustum_cubeInAABB(&level->renderer.chunks[i].aabb)) {
            chunk_render(level, &level->renderer.chunks[i], layer);
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
                if (level_isTile(level, x, y, z)) {
                    glPushName(0);

                    for (int i = 0; i < 6; i++) {
                        glPushName(i);
                        tesselator_init(&level->renderer.tesselator);
                        renderFace(&level->renderer.tesselator, x, y, z, i);
                        tesselator_flush(&level->renderer.tesselator);
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
    tesselator_init(&renderer->tesselator);
    renderFace(&renderer->tesselator, h->x, h->y, h->z, h->f);
    tesselator_flush(&renderer->tesselator);
    glDisable(GL_BLEND);
}

void setDirty(LevelRenderer* renderer, int x0, int y0, int z0, int x1, int y1, int z1) {
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

    for (int x = x0; x <= x1; ++x) {
        for (int y = y0; y <= y1; ++y) {
            for (int z = z0; z <= z1; ++z) {
                renderer->chunks[(x + y * renderer->xChunks) * renderer->zChunks + z].dirty = 1;
            }
        }
    }

}

void levelrenderer_tileChanged(LevelRenderer* renderer, int x, int y, int z) {
    setDirty(renderer, x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void levelrenderer_lightColumnChanged(LevelRenderer* renderer, int x, int z, int y0, int y1) {
    setDirty(renderer, x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
}

void levelrenderer_allChanged(Level* level) {
    setDirty(&level->renderer, 0, 0, 0, level->width, level->depth, level->height);
}
