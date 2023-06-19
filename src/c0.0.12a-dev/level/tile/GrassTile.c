#include "GrassTile.h"

#include "Tile.h"
#include <stdlib.h>

int grasstile_getTexture(int face) {
    return face == 1 ? 0 : (face == 0 ? 2 : 3);
}

void grasstile_tick(Level* level, int x, int y, int z) {
    if (!level_isLit(level, x, y, z)) {
        level_setTile(level, x, y, z, tile_dirt.id);
    } else {
        for (int i = 0; i < 4; ++i) {
            int xt = x + rand() % 3 - 1;
            int yt = y + rand() % 5 - 3;
            int zt = z + rand() % 3 - 1;

            if (level_getTile(level, xt, yt, zt) == tile_dirt.id && level_isLit(level, xt, yt, zt)) {
                level_setTile(level, xt, yt, zt, tile_grass.id);
            }
        }
    }
}
