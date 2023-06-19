#include "Bush.h"

#include "Tile.h"

void bush_tick(Level* level, int x, int y, int z) {
    int below = level_getTile(level, x, y - 1, z);

    if (!level_isLit(level, x, y, z) || below != tile_dirt.id && below != tile_grass.id) {
        level_setTile(level, x, y, z, 0);
    }
}

void bush_render(Level* level, int layer, int x, int y, int z) {
    if (!(level_isLit(level, x, y, z) ^ layer != 1)) {
        int tex = tile_bush.tex;
        float u0 = (float)(tex % 16) / 16.0F;
        float u1 = u0 + 0.999F / 16.0F;
        float v0 = (float)(tex / 16) / 16.0F;
        float v1 = v0 + 0.999F / 16.0F;
        unsigned char rots = 2;
        tesselator_color(1.0F, 1.0F, 1.0F);

        for (int r = 0; r < rots; ++r) {
            float xa = (float)(sin((double)r * M_PI / (double)rots + M_PI * 0.25) * 0.5);
            float za = (float)(cos((double)r * M_PI / (double)rots + M_PI * 0.25) * 0.5);
            float x0 = (float)x + 0.5F - xa;
            float x1 = (float)x + 0.5F + xa;
            float y0 = (float)y + 0.0F;
            float y1 = (float)y + 1.0F;
            float z0 = (float)z + 0.5F - za;
            float z1 = (float)z + 0.5F + za;
            tesselator_vertexUV(x0, y1, z0, u1, v0);
            tesselator_vertexUV(x1, y1, z1, u0, v0);
            tesselator_vertexUV(x1, y0, z1, u0, v1);
            tesselator_vertexUV(x0, y0, z0, u1, v1);
            tesselator_vertexUV(x1, y1, z1, u0, v0);
            tesselator_vertexUV(x0, y1, z0, u1, v0);
            tesselator_vertexUV(x0, y0, z0, u1, v1);
            tesselator_vertexUV(x1, y0, z1, u0, v1);
        }
    }
}