#include "ZombieModel.h"

#include <math.h>

void zombiemodel_create(ZombieModel* zombieModel) {
    cube_create(&zombieModel->head, 0, 0);
    cube_addBox(&zombieModel->head, -4.0F, -8.0F, -4.0F, 8, 8, 8);
    cube_create(&zombieModel->body, 16, 16);
    cube_addBox(&zombieModel->body, -4.0F, 0.0F, -2.0F, 8, 12, 4);
    cube_create(&zombieModel->arm0, 40, 16);
    cube_addBox(&zombieModel->arm0, -3.0F, -2.0F, -2.0F, 4, 12, 4);
    cube_setPos(&zombieModel->arm0, -5.0F, 2.0F, 0.0F);
    cube_create(&zombieModel->arm1, 40, 16);
    cube_addBox(&zombieModel->arm1, -1.0F, -2.0F, -2.0F, 4, 12, 4);
    cube_setPos(&zombieModel->arm1, 5.0F, 2.0F, 0.0F);
    cube_create(&zombieModel->leg0, 0, 16);
    cube_addBox(&zombieModel->leg0, -2.0F, 0.0F, -2.0F, 4, 12, 4);
    cube_setPos(&zombieModel->leg0, -2.0F, 12.0F, 0.0F);
    cube_create(&zombieModel->leg1, 0, 16);
    cube_addBox(&zombieModel->leg1, -2.0F, 0.0F, -2.0F, 4, 12, 4);
    cube_setPos(&zombieModel->leg1, 2.0F, 12.0F, 0.0F);
}

void zombiemodel_render(ZombieModel* zombieModel, float time) {
    zombieModel->head.yRot = (float)sin(time * 0.83) * 1.0F;
    zombieModel->head.xRot = (float)sin(time) * 0.8F;
    zombieModel->arm0.xRot = (float)sin(time * 0.6662 + M_PI) * 2.0F;
    zombieModel->arm0.zRot = (float)(sin(time * 0.2312) + 1.0) * 1.0F;
    zombieModel->arm1.xRot = (float)sin(time * 0.6662) * 2.0F;
    zombieModel->arm1.zRot = (float)(sin(time * 0.2812) - 1.0) * 1.0F;
    zombieModel->leg0.xRot = (float)sin(time * 0.6662) * 1.4F;
    zombieModel->leg1.xRot = (float)sin(time * 0.6662 + M_PI) * 1.4F;
    cube_render(&zombieModel->head);
    cube_render(&zombieModel->body);
    cube_render(&zombieModel->arm0);
    cube_render(&zombieModel->arm1);
    cube_render(&zombieModel->leg0);
    cube_render(&zombieModel->leg1);
}