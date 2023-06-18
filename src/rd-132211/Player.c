#include "Player.h"

#include "Util.h"

#include <stdlib.h>
#include <math.h>

void resetPos(Player* player);
void setPos(Player* player, int x, int y, int z);
void movePlayer(Player* player, float xa, float ya, float za);
void moveRelative(Player* player, float xa, float ya, float speed);

void player_create(Player* player, Level* level) {
    player->keys.isW = 0;
    player->keys.isA = 0;
    player->keys.isS = 0;
    player->keys.isD = 0;
    player->keys.isSpace = 0;

    player->level = level;
    player->onGround = 0;
    resetPos(player);
}

void resetPos(Player* player) {
    float x = (float)frand * player->level->width;
    float y = player->level->depth + 10;
    float z = (float)frand * player->level->height;

    setPos(player, x, y, z);
}

void setPos(Player* player, int x, int y, int z) {
    player->x = x;
    player->y = y;
    player->z = z;

    float w = 0.3f;
    float h = 0.9f;

    aabb_create(&player->bb, x - w, y - h, z - w, x + w, y + h, z + w);
}

void turnPlayer(Player* player, float xo, float yo) {
    player->yRot = (float)((double)player->yRot + (double)xo * 0.15);
    player->xRot = (float)((double)player->xRot - (double)yo * 0.15);

    if (player->xRot < -90.0F) {
        player->xRot = -90.0F;
    }

    if (player->xRot > 90.0F) {
        player->xRot = 90.0F;
    }
}

void tick(Player* player) {
    player->xo = player->x;
    player->yo = player->y;
    player->zo = player->z;
    float xa = 0.0F;
    float ya = 0.0F;

    if (player->keys.isW) {
        --ya;
    }

    if (player->keys.isS) {
        ++ya;
    }

    if (player->keys.isA) {
        --xa;
    }

    if (player->keys.isD) {
        ++xa;
    }

    if (player->keys.isSpace && player->onGround) {
        player->yd = 0.12f;
    }

    moveRelative(player, xa, ya, player->onGround ? 0.02f : 0.005f);
    player->yd = (player->yd - 0.005);
    movePlayer(player, player->xd, player->yd, player->zd);
    player->xd *= 0.91F;
    player->yd *= 0.98F;
    player->zd *= 0.91F;

    if (player->onGround) {
        player->xd *= 0.8F;
        player->zd *= 0.8F;
    }
}

void movePlayer(Player* player, float xa, float ya, float za) {
    float xaOrg = xa;
    float yaOrg = ya;
    float zaOrg = za;

    AABB expandedAABB;
    aabb_expand(&expandedAABB, &player->bb, xa, ya, za);
    AABBArray* aABBs = level_getCubes(player->level, &expandedAABB);

    int i;
    for (i = 0; i < aABBs->size; i++) {
        ya = aabb_clipYCollide(&aABBs->arr[i], &player->bb, ya);
    }

    aabb_move(&player->bb, 0.0F, ya, 0.0F);

    for (i = 0; i < aABBs->size; i++) {
        xa = aabb_clipXCollide(&aABBs->arr[i], &player->bb, xa);
    }

    aabb_move(&player->bb, xa, 0.0F, 0.0F);

    for (i = 0; i < aABBs->size; i++) {
        za = aabb_clipZCollide(&aABBs->arr[i], &player->bb, za);
    }

    free(aABBs);

    aabb_move(&player->bb, 0.0f, 0.0f, za);
    player->onGround = yaOrg != ya && yaOrg < 0.0F;

    if (xaOrg != xa) {
        player->xd = 0.0F;
    }

    if (yaOrg != ya) {
        player->yd = 0.0F;
    }

    if (zaOrg != za) {
        player->zd = 0.0F;
    }

    player->x = (player->bb.x0 + player->bb.x1) / 2.0f;
    player->y = player->bb.y0 + 1.62f;
    player->z = (player->bb.z0 + player->bb.z1) / 2.0f;
}

void moveRelative(Player* player, float xa, float za, float speed) {
    float dist = xa * xa + za * za;

    if (dist >= 0.01F) {
        dist = speed / (float)sqrt((double)dist);
        xa *= dist;
        za *= dist;
        float sinNew = (float)sin((double)player->yRot * (double)M_PI / 180.0);
        float cosNew = (float)cos((double)player->yRot * (double)M_PI / 180.0);
        player->xd += xa * cosNew - za * sinNew;
        player->zd += za * cosNew + xa * sinNew;
    }
}
