#ifndef TESSELATOR_H
#define TESSELATOR_H

typedef struct Tesselator {
    float buffer[524288];
    int vertices;
    float u;
	float v;
	float r;
	float g;
	float b;
	int hasColor;
	int hasTexture;
    int len;
    int p;
} Tesselator;

extern Tesselator tesselator_instance;

void tesselator_create();
void tesselator_flush();
void tesselator_clear();
void tesselator_init();
void tesselator_tex(float u, float v);
void tesselator_color(float r, float g, float b);
void tesselator_vertex(float x, float y, float z);
void tesselator_vertexUV(float x, float y, float z, float u, float v);

#endif
