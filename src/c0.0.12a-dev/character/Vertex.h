#ifndef VERTEX_H
#define VERTEX_H

#include "Vec3.h"

typedef struct Vertex {
    Vec3 pos;
    float u;
    float v;
} Vertex;

void vertex_remap(Vertex* source, float u, float v);
void vertex_create(Vertex* source, float x, float y, float z, float u, float v);

#endif
