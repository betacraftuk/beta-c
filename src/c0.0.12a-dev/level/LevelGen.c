#include "LevelGen.h"

#include "../Util.h"
#include "NoiseMap.h"
#include "tile/Tile.h"

#include <stdlib.h>
#include <math.h>

unsigned char* levelgen_generateMap(int width, int height, int depth) {
    int w = width;
    int h = height;
    int d = depth;

    NoiseMap p1, p2, p3, p4;
    noisemap_create(&p1, 0);
    noisemap_create(&p2, 0);
    noisemap_create(&p3, 1);
    noisemap_create(&p4, 1);

    int* heightmap1 = noisemap_read(&p1, w, h);
    int* heightmap2 = noisemap_read(&p2, w, h);    
    int* cf = noisemap_read(&p3, w, h);
    int* rockMap = noisemap_read(&p4, w, h);

    unsigned char* blocks = malloc(sizeof(unsigned char) * (width * height * depth));

    int count;
    int i;
    int length;
    for (count = 0; count < w; ++count) {
        for (i = 0; i < d; ++i) {
            for (int x = 0; x < h; ++x) {
                int y = heightmap1[count + x * width];
                int z = heightmap2[count + x * width];
                length = cf[count + x * width];

                if (length < 128) {
                    z = y;
                }

                int dir1 = y;
                if (z > y) {
                    dir1 = z;
                }

                dir1 = dir1 / 8 + d / 3;
                int dira1 = rockMap[count + x * width] / 8 + d / 3;
                if (dira1 > dir1 - 2) {
                    dira1 = dir1 - 2;
                }

                int dir2 = (i * height + x) * width + count;
                int dira2 = 0;
                if (i == dir1) {
                    dira2 = tile_grass.id;
                }

                if (i < dir1) {
                    dira2 = tile_dirt.id;
                }

                if (i <= dira1) {
                    dira2 = tile_rock.id;
                }

                blocks[dir2] = (unsigned char)dira2;
            }
        }
    }

    count = w * h * d / 256 / 64;

    for (i = 0; i < count; ++i) {
        float var29 = frand * (float)w;
        float var30 = frand * (float)d;
        float var31 = frand * (float)h;
        length = (int)(frand + frand * 150.0F);
        float var32 = (float)((double)frand * M_PI * 2.0);
        float var33 = 0.0F;
        float var34 = (float)((double)frand * M_PI * 2.0);
        float var35 = 0.0F;

        for (int l = 0; l < length; ++l) {
            var29 = (float)((double)var29 + sin((double)var32) * cos((double)var34));
            var31 = (float)((double)var31 + cos((double)var32) * cos((double)var34));
            var30 = (float)((double)var30 + sin((double)var34));
            var32 += var33 * 0.2F;
            var33 *= 0.9F;
            var33 += frand - frand;
            var34 += var35 * 0.5F;
            var34 *= 0.5F;
            var35 *= 0.9F;
            var35 += frand - frand;
            float size = (float)(sin((double)l * M_PI / (double)length) * 2.5 + 1.0);

            for (int xx = (int)(var29 - size); xx <= (int)(var29 + size); ++xx) {
                for (int yy = (int)(var30 - size); yy <= (int)(var30 + size); ++yy) {
                    for (int zz = (int)(var31 - size); zz <= (int)(var31 + size); ++zz) {
                        float xd = (float)xx - var29;
                        float yd = (float)yy - var30;
                        float zd = (float)zz - var31;
                        float dd = xd * xd + yd * yd * 2.0F + zd * zd;
                        if (dd < size * size && xx >= 1 && yy >= 1 && zz >= 1 && xx < width - 1 && yy < depth - 1 && zz < height - 1) {
                            int ii = (yy * height + zz) * width + xx;
                            if (blocks[ii] == tile_rock.id) {
                                blocks[ii] = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    free(heightmap1);
    free(heightmap2);
    free(cf);
    free(rockMap);

    return blocks;
}