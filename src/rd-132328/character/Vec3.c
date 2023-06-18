#include "Vec3.h"

void vec3_set(Vec3* vec, float x, float y, float z) {
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

void vec3_interpolateTo(Vec3* source, Vec3* vec, float p) {
    source->x += (vec->x - source->x) * p;
    source->y += (vec->y - source->y) * p;
    source->z += (vec->z - source->z) * p;
}