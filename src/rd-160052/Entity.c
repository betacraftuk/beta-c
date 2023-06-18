#include "Entity.h"

#include "Util.h"

#include <stdlib.h>
#include <math.h>

void entity_create(Entity* entity, Level* level) {
    entity->level = level;
    entity->onGround = 0;
    entity->removed = 0;
    entity->heightOffset = 0.0;
    entity->bbWidth = 0.6f;
    entity->bbHeight = 1.8f;
    entity_resetPos(entity);
}

void entity_resetPos(Entity* entity) {
    float x = (float)frand * entity->level->width;
    float y = entity->level->depth + 10;
    float z = (float)frand * entity->level->height;

    entity_setPos(entity, x, y, z);
}

void entity_setSize(Entity* entity, float w, float h) {
    entity->bbWidth = w;
    entity->bbHeight = w;
}

void entity_setPos(Entity* entity, float x, float y, float z) {
    entity->x = x;
    entity->y = y;
    entity->z = z;

    float w = entity->bbWidth / 2.0f;
    float h = entity->bbHeight / 2.0f;

    aabb_create(&entity->bb, x - w, y - h, z - w, x + w, y + h, z + w);
}

void entity_turn(Entity* entity, float xo, float yo) {
    entity->yRot = (float)((double)entity->yRot + (double)xo * 0.15);
    entity->xRot = (float)((double)entity->xRot - (double)yo * 0.15);

    if (entity->xRot < -90.0F) {
        entity->xRot = -90.0F;
    }

    if (entity->xRot > 90.0F) {
        entity->xRot = 90.0F;
    }
}

void entity_tick(Entity* entity) {
    entity->xo = entity->x;
    entity->yo = entity->y;
    entity->zo = entity->z;
}

void entity_move(Entity* entity, float xa, float ya, float za) {
    float xaOrg = xa;
    float yaOrg = ya;
    float zaOrg = za;

    AABB expandedAABB;
    aabb_expand(&expandedAABB, &entity->bb, xa, ya, za);
    AABBArray* aABBs = level_getCubes(entity->level, &expandedAABB);

    int i;
    for (i = 0; i < aABBs->size; i++) {
        ya = aabb_clipYCollide(&aABBs->arr[i], &entity->bb, ya);
    }

    aabb_move(&entity->bb, 0.0F, ya, 0.0F);

    for (i = 0; i < aABBs->size; i++) {
        xa = aabb_clipXCollide(&aABBs->arr[i], &entity->bb, xa);
    }

    aabb_move(&entity->bb, xa, 0.0F, 0.0F);

    for (i = 0; i < aABBs->size; i++) {
        za = aabb_clipZCollide(&aABBs->arr[i], &entity->bb, za);
    }

    free(aABBs);

    aabb_move(&entity->bb, 0.0f, 0.0f, za);
    entity->onGround = yaOrg != ya && yaOrg < 0.0F;

    if (xaOrg != xa) {
        entity->xd = 0.0F;
    }

    if (yaOrg != ya) {
        entity->yd = 0.0F;
    }

    if (zaOrg != za) {
        entity->zd = 0.0F;
    }

    entity->x = (entity->bb.x0 + entity->bb.x1) / 2.0f;
    entity->y = entity->bb.y0 + entity->heightOffset;
    entity->z = (entity->bb.z0 + entity->bb.z1) / 2.0f;
}

void entity_moveRelative(Entity* entity, float xa, float za, float speed) {
    float dist = xa * xa + za * za;

    if (dist >= 0.01F) {
        dist = speed / (float)sqrt((double)dist);
        xa *= dist;
        za *= dist;
        float sinNew = (float)sin((double)entity->yRot * (double)M_PI / 180.0);
        float cosNew = (float)cos((double)entity->yRot * (double)M_PI / 180.0);
        entity->xd += xa * cosNew - za * sinNew;
        entity->zd += za * cosNew + xa * sinNew;
    }
}

int entity_isLit(Entity* entity) {
    int xTile = entity->x;
    int yTile = entity->y;
    int zTile = entity->z;
    return level_isLit(entity->level, xTile, yTile, zTile);
}
