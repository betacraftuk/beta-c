#include "Level.h"

#include "LevelRenderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

void level_calcLightDepths(Level* level, int x0, int y0, int x1, int y1);
void level_load(Level* level);

void level_create(Level* level, int w, int h, int d) {
    level->width = w;
    level->height = h;
    level->depth = d;
    level->blocks = malloc(sizeof(uint8_t) * (w * h * d));
    level->blocksSize = 0;
    level->lightDepths = malloc(sizeof(int) * (w * h));

    for (int i = 0; i < (w * h); i++) {
        level->lightDepths[i] = 0;
    }

    levelrenderer_create(&level->renderer, level);

    int i = 0;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < d; y++) {
            for (int z = 0; z < h; z++) {
                i = (y * level->height + z) * level->width + x;
                level->blocks[i] = (uint8_t)(y <= d * 2 / 3 ? 1 : 0);
            }
        }
    }

    level->blocksSize = i + 1;

    level_calcLightDepths(level, 0, 0, w, h);
    level_load(level);
}

void level_load(Level* level) {
    gzFile file = gzopen("level.dat", "rb");

    if (file == NULL) return;

    gzread(file, level->blocks, sizeof(uint8_t) * (level->width * level->height * level->depth));
    gzclose(file);
}

void level_save(Level* level) {
    gzFile file = gzopen("level.dat", "wb");
    gzwrite(file, level->blocks, level->blocksSize);
    gzclose(file);
}

void level_calcLightDepths(Level* level, int x0, int y0, int x1, int y1) {
    for (int x = x0; x < x0 + x1; x++) {
        for (int z = y0; z < y0 + y1; z++) {
            int oldDepth = level->lightDepths[x + z * level->width];

            int y = 0;
            for (y = level->depth - 1; y > 0 && !level_isTile(level, x, y, z); --y) { }

            level->lightDepths[x + z * level->width] = y;

            if (oldDepth != y) {
                int yl0 = oldDepth < y ? oldDepth : y;
                int yl1 = oldDepth > y ? oldDepth : y;

                levelrenderer_lightColumnChanged(&level->renderer, x, z, yl0, yl1);
            }
        }
    }
}

int level_isTile(Level* level, int x, int y, int z) {
    if (x >= 0 && y >= 0 && z >= 0 && x < level->width && y < level->depth && z < level->height) {
         return level->blocks[(y * level->height + z) * level->width + x] == 1;
      } else {
         return 0;
      }
}

AABBArray* level_getCubes(Level* level, AABB* aABB) {
    AABBArray* aabbArray = malloc(sizeof(AABBArray));
    aabbArray->size = 0;

    int x0 = aABB->x0;
    int x1 = aABB->x1 + 1.0F;
    int y0 = aABB->y0;
    int y1 = aABB->y1 + 1.0F;
    int z0 = aABB->z0;
    int z1 = aABB->z1 + 1.0F;

    if (x0 < 0) {
        x0 = 0;
    }

    if (y0 < 0) {
        y0 = 0;
    }

    if (z0 < 0) {
        z0 = 0;
    }

    if (x1 > level->width) {
        x1 = level->width;
    }

    if (y1 > level->depth) {
        y1 = level->depth;
    }

    if (z1 > level->height) {
        z1 = level->height;
    }

    for (int x = x0; x < x1; x++) {
        for (int y = y0; y < y1; y++) {
            for (int z = z0; z < z1; z++) {
                if (level_isTile(level, x, y, z)) {
                    aabb_create(&aabbArray->arr[aabbArray->size], x, y, z, x + 1, y + 1, z + 1);
                    aabbArray->size++;
                }
            }
        }
    }

    return aabbArray;
}

float level_getBrightness(Level* level, int x, int y, int z) {
    float dark = 0.8F;
    float light = 1.0F;
    return x >= 0 && y >= 0 && z >= 0 && x < level->width && y < level->depth && z < level->height ? (y < level->lightDepths[x + z * level->width] ? dark : light) : light;
}

void level_setTile(Level* level, int x, int y, int z, int type) {
    if (x >= 0 && y >= 0 && z >= 0 && x < level->width && y < level->depth && z < level->height) {
        level->blocks[(y * level->height + z) * level->width + x] = (unsigned char)type;
        level_calcLightDepths(level, x, z, 1, 1);

        levelrenderer_tileChanged(&level->renderer, x, y, z);
    }
}
