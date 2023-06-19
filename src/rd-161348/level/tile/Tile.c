#include "Tile.h"

#include "GrassTile.h"
#include "Bush.h"
#include <stdlib.h>

Tile tile_tiles[256];
Tile tile_air;
Tile tile_rock;
Tile tile_grass;
Tile tile_dirt;
Tile tile_stoneBrick;
Tile tile_wood;
Tile tile_bush;

int tile_getTexture(Tile* tile, int face);
int tile_shouldRenderFace(Level* level, int x, int y, int z, int layer);
void tile_renderFace(Tile* tile, int x, int y, int z, int face);

void tile_create(Tile* tile, int id, int tex, int isnull) {
    tile->id = id;
    tile->tex = tex;
    tile->isNull = isnull;
    tile_tiles[id] = *tile;
}

void tile_init() {
    tile_create(&tile_air, 0, -1, 1);
    tile_create(&tile_rock, 1, 1, 0);
    tile_create(&tile_rock, 1, 1, 0);
    tile_create(&tile_grass, 2, 0, 0);
    tile_create(&tile_dirt, 3, 2, 0);
    tile_create(&tile_stoneBrick, 4, 16, 0);
    tile_create(&tile_wood, 5, 4, 0);
    tile_create(&tile_bush, 6, 15, 0);
}

int tile_getTexture(Tile *tile, int face) {
    if (tile->id == 2) {
        return grasstile_getTexture(face);
    } else {
        return tile->tex;
    }
}

void tile_render(Tile* tile, Level* level, int layer, int x, int y, int z) {
    if (tile->id == 6) {
        bush_render(level, layer, x, y, z);
        return;
    }

    float c1 = 1.0F;
    float c2 = 0.8F;
    float c3 = 0.6F;

    if (tile_shouldRenderFace(level, x, y - 1, z, layer)) {
        tesselator_color(c1, c1, c1);
        tile_renderFace(tile, x, y, z, 0);
    }

    if (tile_shouldRenderFace(level, x, y + 1, z, layer)) {
        tesselator_color(c1, c1, c1);
        tile_renderFace(tile, x, y, z, 1);
    }

    if (tile_shouldRenderFace(level, x, y, z - 1, layer)) {
        tesselator_color(c2, c2, c2);
        tile_renderFace(tile, x, y, z, 2);
    }

    if (tile_shouldRenderFace(level, x, y, z + 1, layer)) {
        tesselator_color(c2, c2, c2);
        tile_renderFace(tile, x, y, z, 3);
    }

    if (tile_shouldRenderFace(level, x - 1, y, z, layer)) {
        tesselator_color(c3, c3, c3);
        tile_renderFace(tile, x, y, z, 4);
    }

    if (tile_shouldRenderFace(level, x + 1, y, z, layer)) {
        tesselator_color(c3, c3, c3);
        tile_renderFace(tile, x, y, z, 5);
    }
}

int tile_shouldRenderFace(Level* level, int x, int y, int z, int layer) {
    return !level_isSolidTile(level, x, y, z) && level_isLit(level, x, y, z) ^ layer == 1;
}

void tile_renderFace(Tile* tile, int x, int y, int z, int face) {
    int tex = tile_getTexture(tile, face);
    float u0 = (float)(tex % 16) / 16.0F;
    float u1 = u0 + 0.999F / 16.0F;
    float v0 = (float)(tex / 16) / 16.0F;
    float v1 = v0 + 0.999F / 16.0F;
    float x0 = x + 0.0F;
    float x1 = x + 1.0F;
    float y0 = y + 0.0F;
    float y1 = y + 1.0F;
    float z0 = z + 0.0F;
    float z1 = z + 1.0F;

    if (face == 0) {
        tesselator_vertexUV(x0, y0, z1, u0, v1);
        tesselator_vertexUV(x0, y0, z0, u0, v0);
        tesselator_vertexUV(x1, y0, z0, u1, v0);
        tesselator_vertexUV(x1, y0, z1, u1, v1);
    }

    if (face == 1) {
        tesselator_vertexUV(x1, y1, z1, u1, v1);
        tesselator_vertexUV(x1, y1, z0, u1, v0);
        tesselator_vertexUV(x0, y1, z0, u0, v0);
        tesselator_vertexUV(x0, y1, z1, u0, v1);
    }

    if (face == 2) {
        tesselator_vertexUV(x0, y1, z0, u1, v0);
        tesselator_vertexUV(x1, y1, z0, u0, v0);
        tesselator_vertexUV(x1, y0, z0, u0, v1);
        tesselator_vertexUV(x0, y0, z0, u1, v1);
    }

    if (face == 3) {
        tesselator_vertexUV(x0, y1, z1, u0, v0);
        tesselator_vertexUV(x0, y0, z1, u0, v1);
        tesselator_vertexUV(x1, y0, z1, u1, v1);
        tesselator_vertexUV(x1, y1, z1, u1, v0);
    }

    if (face == 4) {
        tesselator_vertexUV(x0, y1, z1, u1, v0);
        tesselator_vertexUV(x0, y1, z0, u0, v0);
        tesselator_vertexUV(x0, y0, z0, u0, v1);
        tesselator_vertexUV(x0, y0, z1, u1, v1);
    }

    if(face == 5) {
        tesselator_vertexUV(x1, y0, z1, u0, v1);
        tesselator_vertexUV(x1, y0, z0, u1, v1);
        tesselator_vertexUV(x1, y1, z0, u1, v0);
        tesselator_vertexUV(x1, y1, z1, u0, v0);
    }
}

void tile_renderFaceNoTexture(Tile* tile, int x, int y, int z, int face) {
    float x0 = x + 0.0F;
    float x1 = x + 1.0F;
    float y0 = y + 0.0F;
    float y1 = y + 1.0F;
    float z0 = z + 0.0F;
    float z1 = z + 1.0F;

    if (face == 0) {
        tesselator_vertex(x0, y0, z1);
        tesselator_vertex(x0, y0, z0);
        tesselator_vertex(x1, y0, z0);
        tesselator_vertex(x1, y0, z1);
    }

    if (face == 1) {
        tesselator_vertex(x1, y1, z1);
        tesselator_vertex(x1, y1, z0);
        tesselator_vertex(x0, y1, z0);
        tesselator_vertex(x0, y1, z1);
    }

    if (face == 2) {
        tesselator_vertex(x0, y1, z0);
        tesselator_vertex(x1, y1, z0);
        tesselator_vertex(x1, y0, z0);
        tesselator_vertex(x0, y0, z0);
    }

    if (face == 3) {
        tesselator_vertex(x0, y1, z1);
        tesselator_vertex(x0, y0, z1);
        tesselator_vertex(x1, y0, z1);
        tesselator_vertex(x1, y1, z1);
    }

    if (face == 4) {
        tesselator_vertex(x0, y1, z1);
        tesselator_vertex(x0, y1, z0);
        tesselator_vertex(x0, y0, z0);
        tesselator_vertex(x0, y0, z1);
    }

    if (face == 5) {
        tesselator_vertex(x1, y0, z1);
        tesselator_vertex(x1, y0, z0);
        tesselator_vertex(x1, y1, z0);
        tesselator_vertex(x1, y1, z1);
    }

}

void tile_getAABB(Tile* tile, AABB* source, int x, int y, int z) {
    if (tile->id == 6) {
        source->isNull = 1;
        return;
    }

    aabb_create(source, x, y, z, x + 1, y + 1, z + 1);
}

int tile_blocksLight(Tile* tile) {
    if (tile->id == 6) {
        return 0;
    }

    return 1;
}

int tile_isSolid(Tile* tile) {
    if (tile->id == 6) {
        return 0;
    }

    return 1;
}

void tile_tick(Tile* tile, Level* level, int x, int y, int z) {
    if (tile->id == 2) {
        grasstile_tick(level, x, y, z);
    } else if (tile->id == 6) {
        bush_tick(level, x, y, z);
    }
}

void tile_destroy(Tile* tile, Level* level, int x, int y, int z, ParticleEngine* particleEngine) {
    unsigned char SD = 4;

    for (int xx = 0; xx < SD; ++xx) {
        for (int yy = 0; yy < SD; ++yy) {
            for (int zz = 0; zz < SD; ++zz) {
                float xp = (float)x + ((float)xx + 0.5F) / 4.0F;
                float yp = (float)y + ((float)yy + 0.5F) / 4.0F;
                float zp = (float)z + ((float)zz + 0.5F) / 4.0F;

                Particle* particle = malloc(sizeof(Particle));
                particle_create(particle, level, xp, yp, zp, xp - (float)x - 0.5F, yp - (float)y - 0.5F, zp - (float)z - 0.5F, tile->tex);
                util_linkedlist_add(particleEngine->particles, particle);
            }
        }
    }
}
