#ifndef TESSELATOR_H
#define TESSELATOR_H

typedef struct Tesselator {
    float vertexBuffer[300000];
    float texCoordBuffer[200000];
    float colorBuffer[300000];
    int vertices;
    float u;
	float v;
	float r;
	float g;
	float b;
	int hasColor;
	int hasTexture;
} Tesselator;

void tesselator_create(Tesselator* tes);
void tesselator_flush(Tesselator* tes);
void tesselator_clear(Tesselator * tes);
void tesselator_init(Tesselator* tes);
void tesselator_tex(Tesselator* tes, float u, float v);
void tesselator_color(Tesselator* tes, float r, float g, float b);
void tesselator_vertex(Tesselator* tes, float x, float y, float z);

#endif
