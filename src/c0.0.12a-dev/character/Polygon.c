#include "Polygon.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void polygon_create(PolygonB* source, Vertex* vertices, int u0, int v0, int u1, int v1) {
    source->vertexCount = 4;

    vertex_remap(&vertices[0], u1, v0);
    vertex_remap(&vertices[1], u0, v0);
    vertex_remap(&vertices[2], u0, v1);
    vertex_remap(&vertices[3], u1, v1);

    source->vertices[0] = vertices[0];
    source->vertices[1] = vertices[1];
    source->vertices[2] = vertices[2];
    source->vertices[3] = vertices[3];
}

void polygon_render(PolygonB* polygon) {
    glColor3f(1.0F, 1.0F, 1.0F);

    for(int i = 3; i >= 0; --i) {
        Vertex v = polygon->vertices[i];
        glTexCoord2f(v.u / 63.999F, v.v / 31.999F);
        glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    }
}