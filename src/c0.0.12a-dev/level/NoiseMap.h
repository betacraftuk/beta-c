#ifndef NOISEMAP_H
#define NOISEMAP_H

typedef struct NoiseMap {
    int seed;
    int levels;
    int fuzz;
} NoiseMap;

void noisemap_create(NoiseMap* source, int levels);
int* noisemap_read(NoiseMap * source, int width, int height);

#endif
