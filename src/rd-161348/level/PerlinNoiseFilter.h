#ifndef PERLINNOISEFILTER_H
#define PERLINNOISEFILTER_H

typedef struct PerlinNoiseFilter {
    int seed;
    int levels;
    int fuzz;
} PerlinNoiseFilter;

void perlinnoisefilter_create(PerlinNoiseFilter* source, int levels);
int* perlinnoisefilter_read(PerlinNoiseFilter * source, int width, int height);

#endif
