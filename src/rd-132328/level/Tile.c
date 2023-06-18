#include "Tile.h"

#include <stdlib.h>

int tileRock = 0;
int tileGrass = 1;

void renderTile(int tileTex, Tesselator* t, Level* level, int layer, int x, int y, int z) {
    float u0 = (float) (tileTex / 16.0F);
    float u1 = (float) (u0 + 0.999F / 16.0F);
    float v0 = 0.0F;
    float v1 = (float) (v0 + 0.999F / 16.0F);
    float c1 = 1.0;
    float c2 = 0.8;
    float c3 = 0.6;
    float x0 = x + 0.0;
    float x1 = x + 1.0;
    float y0 = y + 0.0;
    float y1 = y + 1.0;
    float z0 = z + 0.0;
    float z1 = z + 1.0;
    float br;

    if (!level_isTile(level, x, y - 1, z)) {
        br = level_getBrightness(level, x, y - 1, z) * c1;
        
        if (br == c1 ^ layer == 1) {
            tesselator_color(t, br, br, br);
            tesselator_tex(t, u0, v1);
            tesselator_vertex(t, x0, y0, z1);
            tesselator_tex(t, u0, v0);
            tesselator_vertex(t, x0, y0, z0);
            tesselator_tex(t, u1, v0);
            tesselator_vertex(t, x1, y0, z0);
            tesselator_tex(t, u1, v1);
            tesselator_vertex(t, x1, y0, z1);
        }
    }

    if (!level_isTile(level, x, y + 1, z)) {
        br = level_getBrightness(level, x, y, z) * c1;

        if (br == c1 ^ layer == 1) {
            tesselator_color(t, br, br, br);
            tesselator_tex(t, u1, v1);
            tesselator_vertex(t, x1, y1, z1);
            tesselator_tex(t, u1, v0);
            tesselator_vertex(t, x1, y1, z0);
            tesselator_tex(t, u0, v0);
            tesselator_vertex(t, x0, y1, z0);
            tesselator_tex(t, u0, v1);
            tesselator_vertex(t, x0, y1, z1);
        }
    }

    if (!level_isTile(level, x, y, z - 1)) {
        br = level_getBrightness(level, x, y, z - 1) * c2;

        if (br == c2 ^ layer == 1) {
            tesselator_color(t, br, br, br);
            tesselator_tex(t, u1, v0);
            tesselator_vertex(t, x0, y1, z0);
            tesselator_tex(t, u0, v0);
            tesselator_vertex(t, x1, y1, z0);
            tesselator_tex(t, u0, v1);
            tesselator_vertex(t, x1, y0, z0);
            tesselator_tex(t, u1, v1);
            tesselator_vertex(t, x0, y0, z0);
        }
    }

    if (!level_isTile(level, x, y, z + 1)) {
        br = level_getBrightness(level, x, y, z + 1) * c2;

        if (br == c2 ^ layer == 1) {
            tesselator_color(t, br, br, br);
            tesselator_tex(t, u0, v0);
            tesselator_vertex(t, x0, y1, z1);
            tesselator_tex(t, u0, v1);
            tesselator_vertex(t, x0, y0, z1);
            tesselator_tex(t, u1, v1);
            tesselator_vertex(t, x1, y0, z1);
            tesselator_tex(t, u1, v0);
            tesselator_vertex(t, x1, y1, z1);
        }
    }

    if (!level_isTile(level, x - 1, y, z)) {
        br = level_getBrightness(level, x - 1, y, z) * c3;

        if (br == c3 ^ layer == 1) {
            tesselator_color(t, br, br, br);
            tesselator_tex(t, u1, v0);
            tesselator_vertex(t, x0, y1, z1);
            tesselator_tex(t, u0, v0);
            tesselator_vertex(t, x0, y1, z0);
            tesselator_tex(t, u0, v1);
            tesselator_vertex(t, x0, y0, z0);
            tesselator_tex(t, u1, v1);
            tesselator_vertex(t, x0, y0, z1);
        }
    }

    if (!level_isTile(level, x + 1, y, z)) {
        br = level_getBrightness(level, x + 1, y, z) * c3;

        if (br == c3 ^ layer == 1) {
            tesselator_color(t, br, br, br);
            tesselator_tex(t, u0, v1);
            tesselator_vertex(t, x1, y0, z1);
            tesselator_tex(t, u1, v1);
            tesselator_vertex(t, x1, y0, z0);
            tesselator_tex(t, u1, v0);
            tesselator_vertex(t, x1, y1, z0);
            tesselator_tex(t, u0, v0);
            tesselator_vertex(t, x1, y1, z1);
        }
    }
}

void renderFace(Tesselator* t, int x, int y, int z, int face) {
    float x0 = x + 0.0;
    float x1 = x + 1.0;
    float y0 = y + 0.0;
    float y1 = y + 1.0;
    float z0 = z + 0.0;
    float z1 = z + 1.0;

    if (face == 0) {
         tesselator_vertex(t, x0, y0, z1);
         tesselator_vertex(t, x0, y0, z0);
         tesselator_vertex(t, x1, y0, z0);
         tesselator_vertex(t, x1, y0, z1);
    }

    if (face == 1) {
         tesselator_vertex(t, x1, y1, z1);
         tesselator_vertex(t, x1, y1, z0);
         tesselator_vertex(t, x0, y1, z0);
         tesselator_vertex(t, x0, y1, z1);
    }

    if (face == 2) {
         tesselator_vertex(t, x0, y1, z0);
         tesselator_vertex(t, x1, y1, z0);
         tesselator_vertex(t, x1, y0, z0);
         tesselator_vertex(t, x0, y0, z0);
    }

    if (face == 3) {
         tesselator_vertex(t, x0, y1, z1);
         tesselator_vertex(t, x0, y0, z1);
         tesselator_vertex(t, x1, y0, z1);
         tesselator_vertex(t, x1, y1, z1);
    }

    if (face == 4) {
         tesselator_vertex(t, x0, y1, z1);
         tesselator_vertex(t, x0, y1, z0);
         tesselator_vertex(t, x0, y0, z0);
         tesselator_vertex(t, x0, y0, z1);
    }

    if (face == 5) {
        tesselator_vertex(t, x1, y0, z1);
        tesselator_vertex(t, x1, y0, z0);
        tesselator_vertex(t, x1, y1, z0);
        tesselator_vertex(t, x1, y1, z1);
    }
}
