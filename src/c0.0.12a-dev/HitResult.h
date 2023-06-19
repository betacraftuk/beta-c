#ifndef HITRESULT_H
#define HITRESULT_H

typedef struct HitResult {
    int type;
	int x;
	int y;
	int z;
	int f;
} HitResult;

void hitresult_create(HitResult* hitResult, int type, int x, int y, int z, int f);

#endif
