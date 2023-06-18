#include "Cube.h"

#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void cube_setTextOffs(Cube* cube, int xTexOffs, int yTexOffs) {
    cube->xTexOffs = xTexOffs;
    cube->yTexOffs = yTexOffs;
}

void cube_createBoxVertexArray(Vertex* source, Vertex* v1, Vertex* v2, Vertex* v3, Vertex* v4) {
    source[0] = *v1;
    source[1] = *v2;
    source[2] = *v3;
    source[3] = *v4;
}

void cube_addBox(Cube* cube, float x0, float y0, float z0, int w, int h, int d) {
    cube->verticesLen = 8;
    cube->polygonsLen = 6;
    float x1 = x0 + w;
    float y1 = y0 + h;
    float z1 = z0 + d;
    vertex_create(&cube->vertices[0], x0, y0, z0, 0.0F, 0.0F);
    vertex_create(&cube->vertices[1], x1, y0, z0, 0.0F, 8.0F);
    vertex_create(&cube->vertices[2], x1, y1, z0, 8.0F, 8.0F);
    vertex_create(&cube->vertices[3], x0, y1, z0, 8.0F, 0.0F);
    vertex_create(&cube->vertices[4], x0, y0, z1, 0.0F, 0.0F);
    vertex_create(&cube->vertices[5], x1, y0, z1, 0.0F, 8.0F);
    vertex_create(&cube->vertices[6], x1, y1, z1, 8.0F, 8.0F);
    vertex_create(&cube->vertices[7], x0, y1, z1, 8.0F, 0.0F);
    Vertex* u0 = &cube->vertices[0];
    Vertex* u1 = &cube->vertices[1];
    Vertex* u2 = &cube->vertices[2];
    Vertex* u3 = &cube->vertices[3];
    Vertex* l0 = &cube->vertices[4];
    Vertex* l1 = &cube->vertices[5];
    Vertex* l2 = &cube->vertices[6];
    Vertex* l3 = &cube->vertices[7];
    Vertex vp[4];
    cube_createBoxVertexArray(vp, l1, u1, u2, l2);
    polygon_create(&cube->polygons[0], vp, cube->xTexOffs + d + w, cube->yTexOffs + d, cube->xTexOffs + d + w + d, cube->yTexOffs + d + h);
    cube_createBoxVertexArray(vp, u0, l0, l3, u3);
    polygon_create(&cube->polygons[1], vp, cube->xTexOffs + 0, cube->yTexOffs + d, cube->xTexOffs + d, cube->yTexOffs + d + h);
    cube_createBoxVertexArray(vp, l1, l0, u0, u1);
    polygon_create(&cube->polygons[2], vp, cube->xTexOffs + d, cube->yTexOffs + 0, cube->xTexOffs + d + w, cube->yTexOffs + d);
    cube_createBoxVertexArray(vp, u2, u3, l3, l2);
    polygon_create(&cube->polygons[3], vp, cube->xTexOffs + d + w, cube->yTexOffs + 0, cube->xTexOffs + d + w + w, cube->yTexOffs + d);
    cube_createBoxVertexArray(vp, u1, u0, u3, u2);
    polygon_create(&cube->polygons[4], vp, cube->xTexOffs + d, cube->yTexOffs + d, cube->xTexOffs + d + w, cube->yTexOffs + d + h);
    cube_createBoxVertexArray(vp, l0, l1, l2, l3);
    polygon_create(&cube->polygons[5], vp, cube->xTexOffs + d + w + d, cube->yTexOffs + d, cube->xTexOffs + d + w + d + w, cube->yTexOffs + d + h);
}

void cube_setPos(Cube* cube, float x, float y, float z) {
    cube->x = x;
    cube->y = y;
    cube->z = z;
}

void cube_render(Cube* cube) {
    float c = 57.29578F;
    glPushMatrix();
    glTranslatef(cube->x, cube->y, cube->z);
    glRotatef(cube->zRot * c, 0.0F, 0.0F, 1.0F);
    glRotatef(cube->yRot * c, 0.0F, 1.0F, 0.0F);
    glRotatef(cube->xRot * c, 1.0F, 0.0F, 0.0F);
    glBegin(GL_QUADS);

    for (int i = 0; i < cube->polygonsLen; ++i) {
        polygon_render(&cube->polygons[i]);
    }

    glEnd();
    glPopMatrix();
}