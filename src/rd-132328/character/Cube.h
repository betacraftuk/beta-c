#ifndef CUBE_H
#define CUBE_H

#include "Vertex.h"
#include "Polygon.h"

typedef struct Cube {
    Vertex vertices[8];
    PolygonB polygons[6];
    int verticesLen;
    int polygonsLen;
    int xTexOffs;
	int yTexOffs;
	float x;
	float y;
	float z;
	float xRot;
	float yRot;
	float zRot;
} Cube;

void cube_setTextOffs(Cube* cube, int xTexOffs, int yTexOffs);
void cube_addBox(Cube* cube, float x0, float y0, float z0, int w, int h, int d);
void cube_setPos(Cube* cube, float x, float y, float z);
void cube_render(Cube* cube);

#endif
