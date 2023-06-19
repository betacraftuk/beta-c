#ifndef VEC3_H
#define VEC3_H

typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

void vec3_set(Vec3* vec, float x, float y, float z);
void vec3_interpolateTo(Vec3* source, Vec3* vec, float p);

#endif
