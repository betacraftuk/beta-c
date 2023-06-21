#ifndef FONT_H
#define FONT_H

typedef struct Font {
    int charWidths[256];
    int fontTexture;
} Font;

void font_create(Font* font, const char* name);
void font_drawShadow(Font* font, const char* str, int x, int y, int color);

#endif
