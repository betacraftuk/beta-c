#ifndef GRASSTILE_H
#define GRASSTILE_H

#include "../Level.h"

int grasstile_getTexture(int face);
void grasstile_tick(Level* level, int x, int y, int z);

#endif