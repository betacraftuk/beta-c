#ifndef LEVELRENDERER_H
#define LEVELRENDERER_H

#include "../HitResult.h"
#include "../Player.h"

void levelrenderer_create(LevelRenderer* levelRenderer, Level* level);
void levelrenderer_render(Level* render, Player* player, int layer);
void levelrenderer_tileChanged(LevelRenderer* renderer, int x, int y, int z);
void levelrenderer_lightColumnChanged(LevelRenderer* renderer, int x, int z, int y0, int y1);
void levelrenderer_allChanged(Level* level);
void levelrenderer_renderHit(Level* level, HitResult* h, int mode, int tileType);
void levelrenderer_renderPick(Level* level, Player* player);
void levelrenderer_updateDirtyChunks(Level* level, Player* player);

#endif
