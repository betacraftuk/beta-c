#include "AABB.h"

#include <stdlib.h>

#define EPSILON 0.0

void aabb_create(AABB* aabb, float x0, float y0, float z0, float x1, float y1, float z1) {
    aabb->x0 = x0;
    aabb->y0 = y0;
    aabb->z0 = z0;
    aabb->x1 = x1;
    aabb->y1 = y1;
    aabb->z1 = z1;
}

void aabb_expand(AABB* source, AABB* aabb, float xa, float ya, float za) {
    source->x0 = aabb->x0;
    source->y0 = aabb->y0;
    source->z0 = aabb->z0;
    source->x1 = aabb->x1;
    source->y1 = aabb->y1;
    source->z1 = aabb->z1;

    if (xa < 0.0F) {
        source->x0 += xa;
    }

    if (xa > 0.0F) {
        source->x1 += xa;
    }

    if (ya < 0.0F) {
        source->y0 += ya;
    }

    if (ya > 0.0F) {
        source->y1 += ya;
    }

    if (za < 0.0F) {
        source->z0 += za;
    }

    if (za > 0.0F) {
        source->z1 += za;
    }
}

float aabb_clipXCollide(AABB* bb, AABB* c, float xa) {
    if (c->y1 > bb->y0 && c->y0 < bb->y1) {
        if (c->z1 > bb->z0 && c->z0 < bb->z1) {
            float max;
            if (xa > 0.0F && c->x1 <= bb->x0) {
                max = bb->x0 - c->x1 - EPSILON;
                if (max < xa) {
                    xa = max;
                }
            }

            if (xa < 0.0F && c->x0 >= bb->x1) {
                max = bb->x1 - c->x0 + EPSILON;
                if(max > xa) {
                    xa = max;
                }
            }

            return xa;
        } else {
            return xa;
        }
    } else {
        return xa;
    }
}

void aabb_grow(AABB* aabb, AABB* bb, float xa, float ya, float za) {
    aabb->x0 = bb->x0 - xa;
    aabb->y0 = bb->y0 - ya;
    aabb->z0 = bb->z0 - za;
    aabb->x1 = bb->x1 + xa;
    aabb->y1 = bb->y1 + ya;
    aabb->z1 = bb->z1 + za;
}

float aabb_clipYCollide(AABB* bb, AABB* c, float ya) {
    if (c->x1 > bb->x0 && c->x0 < bb->x1) {
        if (c->z1 > bb->z0 && c->z0 < bb->z1) {
            float max;
            if (ya > 0.0F && c->y1 <= bb->y0) {
                max = bb->y0 - c->y1 - EPSILON;
                if (max < ya) {
                    ya = max;
                }
            }

            if (ya < 0.0F && c->y0 >= bb->y1) {
                max = bb->y1 - c->y0 + EPSILON;
                if (max > ya) {
                    ya = max;
                }
            }

            return ya;
        } else {
            return ya;
        }
    } else {
        return ya;
    }
}

float aabb_clipZCollide(AABB* bb, AABB* c, float za) {
    if (c->x1 > bb->x0 && c->x0 < bb->x1) {
        if (c->y1 > bb->y0 && c->y0 < bb->y1) {
            float max;
            if (za > 0.0F && c->z1 <= bb->z0) {
                max = bb->z0 - c->z1 - EPSILON;
                if (max < za) {
                    za = max;
                }
            }

            if (za < 0.0F && c->z0 >= bb->z1) {
                max = bb->z1 - c->z0 + EPSILON;
                if (max > za) {
                    za = max;
                }
            }

            return za;
        } else {
            return za;
        }
    } else {
        return za;
    }
}

void aabb_move(AABB* aabb, float xa, float ya, float za) {
    aabb->x0 += xa;
    aabb->y0 += ya;
    aabb->z0 += za;
    aabb->x1 += xa;
    aabb->y1 += ya;
    aabb->z1 += za;
}