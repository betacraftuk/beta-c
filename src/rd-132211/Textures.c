#include "Textures.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_image.h>
#endif

void textures_bind(int id);
int lastId = -9999999;

int textures_load(const char* resourceName, int mode) {
    SDL_Surface* image = IMG_Load(resourceName);
    
    if (image == NULL) {
        return -1;
    }

    SDL_Surface* converted = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);

    int textureId;
    glGenTextures(1, &textureId);
    textures_bind(textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

#ifdef __aarch64__
    GLenum format = GL_BGRA;
#else
    GLenum format = GL_BGRA_EXT;
#endif
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, converted->w, converted->h, format, GL_UNSIGNED_BYTE, converted->pixels);
    SDL_FreeSurface(image);
    SDL_FreeSurface(converted);

    return textureId;
}

void textures_bind(int id) {
    if (id != lastId) {
        glBindTexture(GL_TEXTURE_2D, id);
        lastId = id;
    }
}
