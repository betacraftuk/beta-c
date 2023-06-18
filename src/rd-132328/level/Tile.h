#ifndef TILE_H
#define TILE_H

#include "Tesselator.h"
#include "Level.h"

extern int tileRock;
extern int tileGrass;

void renderTile(int tileTex, Tesselator* t, Level* level, int layer, int x, int y, int z);
void renderFace(Tesselator* t, int x, int y, int z, int face);

#endif
