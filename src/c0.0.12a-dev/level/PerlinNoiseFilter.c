#include "PerlinNoiseFilter.h"

#include <stdlib.h>

void perlinnoisefilter_create(PerlinNoiseFilter* source, int levels) {
    source->levels = levels;
    source->seed = rand();
    source->fuzz = 16;
}

int* perlinnoisefilter_read(PerlinNoiseFilter* source, int width, int height) {
    int* tmp = malloc(sizeof(int) * (width * height));
    int level = source->levels;
    int result = width >> level;

    int y;
    int x;
    int xd = rand() % 256;
    for (y = 0; y < height; y += result) {
        for (x = 0; x < width; x += result) {
            tmp[x + y * width] = ((rand() % 256) - 128) * source->fuzz;
        }
    }

    for (result = width >> level; result > 1; result /= 2) {
        y = 256 * (result << level);
        x = result / 2;

        int y1;
        int x1;
        int c;
        int r;
        int d;
        int mu;
        int ml;
        for (y1 = 0; y1 < height; y1 += result) {
            for (x1 = 0; x1 < width; x1 += result) {
                c = tmp[(x1 + 0) % width + (y1 + 0) % height * width];
                r = tmp[(x1 + result) % width + (y1 + 0) % height * width];
                d = tmp[(x1 + 0) % width + (y1 + result) % height * width];
                mu = tmp[(x1 + result) % width + (y1 + result) % height * width];
                ml = (c + d + r + mu) / 4 + rand() % (y * 2) - y;
                tmp[x1 + x + (y1 + x) * width] = ml;
            }
        }

        for (y1 = 0; y1 < height; y1 += result) {
            for (x1 = 0; x1 < width; x1 += result) {
                c = tmp[x1 + y1 * width];
                r = tmp[(x1 + result) % width + y1 * width];
                d = tmp[x1 + (y1 + result) % width * width];
                mu = tmp[(x1 + x & width - 1) + (y1 + x - result & height - 1) * width];
                ml = tmp[(x1 + x - result & width - 1) + (y1 + x & height - 1) * width];
                int m = tmp[(x1 + x) % width + (y1 + x) % height * width];
                int u = (c + r + m + mu) / 4 + rand() % (y * 2) - y;
                int l = (c + d + m + ml) / 4 + rand() % (y * 2) - y;
                tmp[x1 + x + y1 * width] = u;
                tmp[x1 + (y1 + x) * width] = l;
            }
        }
    }

    int* var19 = malloc(sizeof(int) * (width * height));

    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            var19[x + y * width] = tmp[x % width + y % height * width] / 512 + 128;
        }
    }

    free(tmp);

    return var19;
}