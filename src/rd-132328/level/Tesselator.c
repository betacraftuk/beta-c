#include "Tesselator.h"

#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void tesselator_clear(Tesselator* tes);

void tesselator_create(Tesselator* tes) {
    tes->vertices = 0;
	tes->hasColor = 0;
	tes->hasTexture = 0;
}

void tesselator_flush(Tesselator* tes) {
    glVertexPointer(3, GL_FLOAT, 0, tes->vertexBuffer);

    if (tes->hasTexture) {
        glTexCoordPointer(2, GL_FLOAT, 0, tes->texCoordBuffer);
    }

    if (tes->hasColor) {
        glColorPointer(3, GL_FLOAT, 0, tes->colorBuffer);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    if (tes->hasTexture) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (tes->hasColor) {
        glEnableClientState(GL_COLOR_ARRAY);
    }

    glDrawArrays(GL_QUADS, GL_POINTS, tes->vertices);
    glDisableClientState(GL_VERTEX_ARRAY);

    if (tes->hasTexture) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (tes->hasColor) {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    tesselator_clear(tes);
}

void tesselator_clear(Tesselator* tes) {
    tes->vertices = 0;
}

void tesselator_init(Tesselator* tes) {
    tesselator_clear(tes);
    tes->hasColor = 0;
    tes->hasTexture = 0;
}

void tesselator_tex(Tesselator* tes, float u, float v) {
    tes->hasTexture = 1;
    tes->u = u;
    tes->v = v;
}

void tesselator_color(Tesselator* tes, float r, float g, float b) {
    tes->hasColor = 1;
    tes->r = r;
    tes->g = g;
    tes->b = b;
}

void tesselator_vertex(Tesselator* tes, float x, float y, float z) {
    tes->vertexBuffer[tes->vertices * 3 + 0] = x;
    tes->vertexBuffer[tes->vertices * 3 + 1] = y;
    tes->vertexBuffer[tes->vertices * 3 + 2] = z;

    if (tes->hasTexture) {
        tes->texCoordBuffer[tes->vertices * 2 + 0] = tes->u;
        tes->texCoordBuffer[tes->vertices * 2 + 1] = tes->v;
    }

    if (tes->hasColor) {
        tes->colorBuffer[tes->vertices * 3 + 0] = tes->r;
        tes->colorBuffer[tes->vertices * 3 + 1] = tes->g;
        tes->colorBuffer[tes->vertices * 3 + 2] = tes->b;
    }

    tes->vertices++;
    if (tes->vertices == 100000) {
        tesselator_flush(tes);
    }
}
