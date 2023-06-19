#include "HitResult.h"

#include <stdlib.h>

void hitresult_create(HitResult* hitResult, int type, int x, int y, int z, int f) {
    hitResult->type = type;
    hitResult->x = x;
    hitResult->y = y;
    hitResult->z = z;
    hitResult->f = f;
}