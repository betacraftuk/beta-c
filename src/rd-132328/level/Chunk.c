#include "Chunk.h"

#include "../Textures.h"
#include "Tile.h"
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void chunk_rebuild(Level* level, Chunk* chunk, int layer);
void chunk_render(Level* level, Chunk* chunk, int layer);

Tesselator chunk_t;
int chunk_tEmpty = 1;
int chunk_rebuiltThisFrame = 0;
int chunk_updates = 0;

void chunk_create(Chunk* chunk, int x0, int y0, int z0, int x1, int y1, int z1) {
    chunk->x0 = x0;
    chunk->y0 = y0;
    chunk->z0 = z0;
    chunk->x1 = x1;
    chunk->y1 = y1;
    chunk->z1 = z1;
    chunk->dirty = 1;
    aabb_create(&chunk->aabb, x0, y0, z0, x1, y1, z1);
    chunk->lists = glGenLists(2);

    if (chunk_tEmpty) {
        tesselator_create(&chunk_t);
        chunk_tEmpty = 0;
    }
}

void chunk_rebuild(Level* level, Chunk* chunk, int layer) {
    if (chunk_rebuiltThisFrame != 2) {
        chunk->dirty = 0;
        chunk_updates++;
        chunk_rebuiltThisFrame++;
        int id = textures_load("terrain.png", GL_NEAREST);
        glNewList(chunk->lists + layer, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, id);
        tesselator_init(&chunk_t);
        int tiles = 0;

        for (int x = chunk->x0; x < chunk->x1; x++) {
            for (int y = chunk->y0; y < chunk->y1; y++) {
                for (int z = chunk->z0; z < chunk->z1; z++) {
                    if (level_isTile(level, x, y, z)) {
                        int tex = y != level->depth * 2 / 3;
                        tiles++;

                        if (!tex) {
                            renderTile(tileRock, &chunk_t, level, layer, x, y, z);
                        } else {
                            renderTile(tileGrass, &chunk_t, level, layer, x, y, z);
                        }
                    }
                }
            }
        }

        tesselator_flush(&chunk_t);
        glDisable(GL_TEXTURE_2D);
        glEndList();
    }
}

void chunk_render(Level* level, Chunk* chunk, int layer) {
    if (chunk->dirty) {
        chunk_rebuild(level, chunk, 0);
        chunk_rebuild(level, chunk, 1);
    }

    glCallList(chunk->lists + layer);
}
