#include "Vertex.h"

void vertex_create(Vertex* source, float x, float y, float z, float u, float v) {
    source->pos.x = x;
    source->pos.y = y;
    source->pos.z = z;
    source->u = u;
    source->v = v;
}

void vertex_remap(Vertex* source, float u, float v) {
    vertex_create(source, source->pos.x, source->pos.y, source->pos.z, u, v);
}
