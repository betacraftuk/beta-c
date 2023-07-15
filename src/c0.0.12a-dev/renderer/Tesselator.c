#include "Tesselator.h"

#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

Tesselator tesselator_instance;

void tesselator_create() {
    tesselator_instance.vertices = 0;
	tesselator_instance.hasColor = 0;
	tesselator_instance.hasTexture = 0;
    tesselator_instance.len = 3;
    tesselator_instance.p = 0;
}

void tesselator_flush() {
    if (tesselator_instance.vertices > 0) {
        if (tesselator_instance.hasTexture && tesselator_instance.hasColor) {
            glInterleavedArrays(GL_T2F_C3F_V3F, 0, tesselator_instance.buffer);
        }
        else if (tesselator_instance.hasTexture) {
            glInterleavedArrays(GL_T2F_V3F, 0, tesselator_instance.buffer);
        }
        else if (tesselator_instance.hasColor) {
            glInterleavedArrays(GL_C3F_V3F, 0, tesselator_instance.buffer);
        }
        else {
            glInterleavedArrays(GL_V3F, 0, tesselator_instance.buffer);
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        if (tesselator_instance.hasTexture) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        if (tesselator_instance.hasColor) {
            glEnableClientState(GL_COLOR_ARRAY);
        }

        glDrawArrays(GL_QUADS, GL_POINTS, tesselator_instance.vertices);
        glDisableClientState(GL_VERTEX_ARRAY);

        if (tesselator_instance.hasTexture) {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        if (tesselator_instance.hasColor) {
            glDisableClientState(GL_COLOR_ARRAY);
        }

        tesselator_clear();
    }
}

void tesselator_clear() {
    tesselator_instance.vertices = 0;
    tesselator_instance.p = 0;
}

void tesselator_init() {
    tesselator_clear();
    tesselator_instance.hasColor = 0;
    tesselator_instance.hasTexture = 0;
    tesselator_instance.noColor = 0;
}

void tesselator_tex(float u, float v) {
    if (!tesselator_instance.hasTexture) {
        tesselator_instance.len += 2;
    }

    tesselator_instance.hasTexture = 1;
    tesselator_instance.u = u;
    tesselator_instance.v = v;
}

void tesselator_color(float r, float g, float b) {
    if (!tesselator_instance.noColor) {
        if (!tesselator_instance.hasColor) {
            tesselator_instance.len += 3;
        }
    }

    tesselator_instance.hasColor = 1;
    tesselator_instance.r = r;
    tesselator_instance.g = g;
    tesselator_instance.b = b;
}

void tesselator_color_code(int c) {
    float r = (float)(c >> 16 & 255) / 255.0F;
    float g = (float)(c >> 8 & 255) / 255.0F;
    float b = (float)(c & 255) / 255.0F;
    tesselator_color(r, g, b);
}

void tesselator_vertexUV(float x, float y, float z, float u, float v) {
    tesselator_tex(u, v);
    tesselator_vertex(x, y, z);
}

void tesselator_vertex(float x, float y, float z) {
    if (tesselator_instance.hasTexture) {
        tesselator_instance.buffer[tesselator_instance.p++] = tesselator_instance.u;
        tesselator_instance.buffer[tesselator_instance.p++] = tesselator_instance.v;
    }

    if (tesselator_instance.hasColor) {
        tesselator_instance.buffer[tesselator_instance.p++] = tesselator_instance.r;
        tesselator_instance.buffer[tesselator_instance.p++] = tesselator_instance.g;
        tesselator_instance.buffer[tesselator_instance.p++] = tesselator_instance.b;
    }

    tesselator_instance.buffer[tesselator_instance.p++] = x;
    tesselator_instance.buffer[tesselator_instance.p++] = y;
    tesselator_instance.buffer[tesselator_instance.p++] = z;

    tesselator_instance.vertices++;
    if (tesselator_instance.vertices % 4 == 0 && tesselator_instance.p >= 524288 - tesselator_instance.len * 4) {
        tesselator_flush();
    }
}
