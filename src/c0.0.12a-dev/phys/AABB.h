#ifndef AABB_H
#define AABB_H

typedef struct AABB {
    int isNull;
	float x0;
	float y0;
	float z0;
	float x1;
	float y1;
	float z1;
} AABB;

typedef struct AABBArray {
    AABB arr[32];
    int size;
} AABBArray;

void aabb_create(AABB* aabb, float x0, float y0, float z0, float x1, float y1, float z1);
void aabb_move(AABB* aabb, float xa, float ya, float za);
void aabb_expand(AABB* source, AABB* aabb, float xa, float ya, float za);
void aabb_grow(AABB* aabb, AABB* bb, float xa, float ya, float za);
float aabb_clipXCollide(AABB* bb, AABB* c, float xa);
float aabb_clipYCollide(AABB* bb, AABB* c, float ya);
float aabb_clipZCollide(AABB* bb, AABB* c, float za);
int aabb_intersects(AABB* source, AABB* c);

#endif
