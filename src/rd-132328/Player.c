#include "Player.h"

#include <stdlib.h>
#include <math.h>

void player_create(Player* player, Level* level) {
    player->keys.isW = 0;
    player->keys.isA = 0;
    player->keys.isS = 0;
    player->keys.isD = 0;
    player->keys.isSpace = 0;

    entity_create(&player->entity, level);
    player->entity.heightOffset = 1.62f;
    resetPos(&player->entity);
}

void player_tick(Player* player) {
    player->entity.xo = player->entity.x;
    player->entity.yo = player->entity.y;
    player->entity.zo = player->entity.z;
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

    if (player->keys.isSpace && player->entity.onGround) {
        player->entity.yd = 0.12f;
    }

    moveRelative(&player->entity, xa, ya, player->entity.onGround ? 0.02f : 0.005f);
    player->entity.yd = (player->entity.yd - 0.005);
    entity_move(&player->entity, player->entity.xd, player->entity.yd, player->entity.zd);
    player->entity.xd *= 0.91F;
    player->entity.yd *= 0.98F;
    player->entity.zd *= 0.91F;

    if (player->entity.onGround) {
        player->entity.xd *= 0.8F;
        player->entity.zd *= 0.8F;
    }
}
