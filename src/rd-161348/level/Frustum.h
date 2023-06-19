#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "../phys/AABB.h"

typedef struct Frustum {
    float m_Frustum[6][4];
	float proj[16];
	float modl[16];
	float clip[16];
} Frustum;

extern Frustum frustum;

int frustum_visibleAABB(AABB* aabb);
void frustum_calculate();

#endif
