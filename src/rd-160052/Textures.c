#include "Textures.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
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

typedef struct HashMap {
    char resourceName[128];
    int id;
} HashMap;

HashMap idMap[256];
int idMapLen = 0;

int textures_load(const char* resourceName, int mode) {
    for (int i = 0; i < idMapLen; i++) {
        if (strcmp(resourceName, idMap[i].resourceName) == 0) {
            return idMap[i].id;
        }
    }

    SDL_Surface* image = IMG_Load(resourceName);
    
    if (image == NULL) {
        return -1;
    }

    SDL_Surface* converted = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);

    int textureId;
    glGenTextures(1, &textureId);

    assert(strlen(resourceName) < 128);

    strcpy(idMap[idMapLen].resourceName, resourceName);
    idMap[idMapLen].id = textureId;
    idMapLen++;

    printf("%s -> %d\n", resourceName, textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
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