#include "Font.h"

#include "../renderer/Textures.h"
#include "../renderer/Tesselator.h"

#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <SDL2/SDL_image.h>
#endif

#define FONT_STRING "0123456789abcdef"

void font_create(Font* font, const char* name) {
    SDL_Surface* image = IMG_Load(name);
    
    if (image == NULL) {
        exit(1);
    }

    for (int i = 0; i < 128; i++) {
        int xt = i % 16;
        int yt = i / 16;
        int x = 0;

        for (int emptyColumn = 0; x < 8 && !emptyColumn; x++) {
            int xPixel = xt * 8 + x;
            emptyColumn = 1;

            for (int y = 0; y < 8 && emptyColumn; y++) {
                int yPixel = (yt * 8 + y) * image->w;

                SDL_LockSurface(image);
                Uint32* pixels = (Uint32*)image->pixels;
                //Uint32 pixel = pixels[xPixel + yPixel] & 255; // error
                SDL_UnlockSurface(image);

                //if (pixel > 128) {
                //    emptyColumn = 0;
                //}
            }
        }

        if (i == 32) {
            x = 4;
        }

        font->charWidths[i] = x;
    }

    font->fontTexture = textures_load(name, 9728);
    SDL_FreeSurface(image);
}

void font_draw(Font* font, const char* str, int x, int y, int color, int darken) {
    if (darken) {
        color = (color & 16579836) >> 2;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font->fontTexture);
    tesselator_init();
    tesselator_color_code(color);
    int xo = 0;

    for (int i = 0; i < strlen(str); ++i) {
        int ix = 0;
        int iy = 0;

        if (str[i] == 38) {
            // check this twice!!!!!!!!!!!!!!!
            char* subs = strstr(FONT_STRING, &str[i + 1]);
            ix = (int)(subs - str);
            printf("%s - index: %d\n", FONT_STRING, ix);
            iy = (ix & 8) * 8;
            int b = (ix & 1) * 191 + iy;
            int g = ((ix & 2) >> 1) * 191 + iy;
            int r = ((ix & 4) >> 2) * 191 + iy;
            color = r << 16 | g << 8 | b;
            i += 2;

            if (darken) {
                color = (color & 16579836) >> 2;
            }

            tesselator_color_code(color);
        }

        ix = str[i] % 16 * 8;
        iy = str[i] / 16 * 8;
        tesselator_vertexUV((float)(x + xo), (float)(y + 8), 0.0F, (float)ix / 128.0F, (float)(iy + 8) / 128.0F);
        tesselator_vertexUV((float)(x + xo + 8), (float)(y + 8), 0.0F, (float)(ix + 8) / 128.0F, (float)(iy + 8) / 128.0F);
        tesselator_vertexUV((float)(x + xo + 8), (float)y, 0.0F, (float)(ix + 8) / 128.0F, (float)iy / 128.0F);
        tesselator_vertexUV((float)(x + xo), (float)y, 0.0F, (float)ix / 128.0F, (float)iy / 128.0F);
        xo += font->charWidths[str[i]];
    }

    tesselator_flush();
    glDisable(GL_TEXTURE_2D);
}

void font_drawShadow(Font* font, const char* str, int x, int y, int color) {
    font_draw(font, str, x + 1, y + 1, color, 1);
    font_draw(font, str, x, y, color, 0);
}