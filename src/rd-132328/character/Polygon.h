#ifndef POLYGON_H
#define POLYGON_H

#include "Vertex.h"

typedef struct PolygonB {
    Vertex vertices[4];
    int vertexCount;
} PolygonB;

void polygon_create(PolygonB* source, Vertex* vertices, int u0, int v0, int u1, int v1);
void polygon_render(PolygonB* polygon);

#endif
