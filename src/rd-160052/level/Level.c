#include "Level.h"

#include "LevelRenderer.h"
#include "PerlinNoiseFilter.h"
#include "tile/Tile.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <zlib.h>

void level_calcLightDepths(Level* level, int x0, int y0, int x1, int y1);
int level_load(Level* level);
void level_generateMap(Level* level);

void level_create(Level* level, int w, int h, int d) {
    level->width = w;
    level->height = h;
    level->depth = d;
    level->blocksSize = sizeof(uint8_t) * (w * h * d);
    level->blocks = malloc(level->blocksSize);
    level->lightDepths = malloc(sizeof(int) * (w * h));
    level->unprocessed = 0;

    for (int i = 0; i < (w * h); i++) {
        level->lightDepths[i] = 0;
    }

    int mapLoaded = level_load(level);

    if (!mapLoaded) {
        level_generateMap(level);
    }

    level_calcLightDepths(level, 0, 0, w, h);
    levelrenderer_create(&level->renderer, level);
}

void level_generateMap(Level* level) {
    int w = level->width;
    int h = level->height;
    int d = level->depth;
    PerlinNoiseFilter p1, p2, p3, p4;
    perlinnoisefilter_create(&p1, 0);
    perlinnoisefilter_create(&p2, 0);
    perlinnoisefilter_create(&p3, 1);
    perlinnoisefilter_create(&p4, 1);

    int* heightmap1 = perlinnoisefilter_read(&p1, w, h);
    int* heightmap2 = perlinnoisefilter_read(&p2, w, h);    
    int* cf = perlinnoisefilter_read(&p3, w, h);
    int* rockMap = perlinnoisefilter_read(&p4, w, h);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < d; y++) {
            for (int z = 0; z < h; z++) {
                int dh1 = heightmap1[x + z * level->width];
                int dh2 = heightmap2[x + z * level->width];
                int cfh = cf[x + z * level->width];

                if (cfh < 128) {
                    dh2 = dh1;
                }

                int dh = dh1;
                if (dh2 > dh1) {
                    dh = dh2;
                }

                dh = dh / 8 + d / 3;
                int rh = rockMap[x + z * level->width] / 8 + d / 3;
                if (rh > dh - 2) {
                    rh = dh - 2;
                }

                int i = (y * level->height + z) * level->width + x;
                int id = 0;
                if (y == dh) {
                    id = tile_grass.id;
                }

                if (y < dh) {
                    id = tile_dirt.id;
                }

                if (y <= rh) {
                    id = tile_rock.id;
                }

                level->blocks[i] = (unsigned char)id;
            }
        }
    }

    free(heightmap1);
    free(heightmap2);
    free(cf);
    free(rockMap);
}

int level_load(Level* level) {
    gzFile file = gzopen("level.dat", "rb");

    if (file == NULL) return 0;

    gzread(file, level->blocks, level->blocksSize);
    gzclose(file);

    return 1;
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
            for (y = level->depth - 1; y > 0 && !level_isSolidTile(level, x, y, z); --y) { }

            level->lightDepths[x + z * level->width] = y;

            if (oldDepth != y) {
                int yl0 = oldDepth < y ? oldDepth : y;
                int yl1 = oldDepth > y ? oldDepth : y;

                levelrenderer_lightColumnChanged(&level->renderer, x, z, yl0, yl1);
            }
        }
    }
}

int level_isLightBlocker(Level* level, int x, int y, int z) {
    Tile* tile = &tile_tiles[level_getTile(level, x, y, z)];
    return !tile->isNull;
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
                Tile* tile = &tile_tiles[level_getTile(level, x, y, z)];

                if (!tile->isNull) {
                    tile_getAABB(&aabbArray->arr[aabbArray->size], x, y, z);
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

int level_setTile(Level* level, int x, int y, int z, int type) {
    if (x >= 0 && y >= 0 && z >= 0 && x < level->width && y < level->depth && z < level->height) {
        if (type == level->blocks[(y * level->height + z) * level->width + x]) {
            return 0;
        } else {
            level->blocks[(y * level->height + z) * level->width + x] = (unsigned char)type;
            level_calcLightDepths(level, x, z, 1, 1);

            levelrenderer_tileChanged(&level->renderer, x, y, z);

            return 1;
        }
    }
    
    return 0;
}

int level_isLit(Level* level, int x, int y, int z) {
    return x >= 0 && y >= 0 && z >= 0 && x < level->width && y < level->depth && z < level->height ? y >= level->lightDepths[x + z * level->width] : 1;
}

int level_getTile(Level* level, int x, int y, int z) {
    return x >= 0 && y >= 0 && z >= 0 && x < level->width && y < level->depth && z < level->height ? level->blocks[(y * level->height + z) * level->width + x] : 0;
}

int level_isSolidTile(Level* level, int x, int y, int z) {
    Tile* tile = &tile_tiles[level_getTile(level, x, y, z)];
    return !tile->isNull;
}

void level_tick(Level* level) {
    level->unprocessed += level->width * level->height * level->depth;
    int ticks = level->unprocessed / 400;
    level->unprocessed -= ticks * 400;

    for (int i = 0; i < ticks; i++) {
        int x = rand() % (level->width);
        int y = rand() % (level->depth);
        int z = rand() % (level->height);
        Tile* tile = &tile_tiles[level_getTile(level, x, y, z)];

        if (!tile->isNull) {
            tile_tick(tile, level, x, y, z);
        }
    }
}
