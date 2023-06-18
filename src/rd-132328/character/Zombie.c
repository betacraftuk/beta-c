#include "Zombie.h"

#include "../Util.h"
#include "../Textures.h"
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void zombie_create(Zombie* zombie, Level* level, float x, float y, float z) {
    entity_create(&zombie->entity, level);
    zombie->entity.x = x;
    zombie->entity.y = y;
    zombie->entity.z = z;
    zombie->timeOffs = (float)frand * 1239813.0F;
    zombie->rot = (float)(frand * M_PI * 2.0);
    zombie->speed = 1.0F;
    cube_setTextOffs(&zombie->head, 0, 0);
    cube_addBox(&zombie->head, -4.0F, -8.0F, -4.0F, 8, 8, 8);
    cube_setTextOffs(&zombie->body, 16, 16);
    cube_addBox(&zombie->body, -4.0F, 0.0F, -2.0F, 8, 12, 4);
    cube_setTextOffs(&zombie->arm0, 40, 16);
    cube_addBox(&zombie->arm0, -3.0F, -2.0F, -2.0F, 4, 12, 4);
    cube_setPos(&zombie->arm0, -5.0F, 2.0F, 0.0F);
    cube_setTextOffs(&zombie->arm1, 40, 16);
    cube_addBox(&zombie->arm1, -1.0F, -2.0F, -2.0F, 4, 12, 4);
    cube_setPos(&zombie->arm1, 5.0F, 2.0F, 0.0F);
    cube_setTextOffs(&zombie->leg0, 0, 16);
    cube_addBox(&zombie->leg0, -2.0F, 0.0F, -2.0F, 4, 12, 4);
    cube_setPos(&zombie->leg0, -2.0F, 12.0F, 0.0F);
    cube_setTextOffs(&zombie->leg1, 0, 16);
    cube_addBox(&zombie->leg1, -2.0F, 0.0F, -2.0F, 4, 12, 4);
    cube_setPos(&zombie->leg1, 2.0F, 12.0F, 0.0F);
}

void zombie_tick(Zombie* zombie) {
    zombie->entity.xo = zombie->entity.x;
    zombie->entity.yo = zombie->entity.y;
    zombie->entity.zo = zombie->entity.z;
    float xa = 0.0F;
    float ya = 0.0F;
    zombie->rot += zombie->rotA;
    zombie->rotA = (float)((double)zombie->rotA * 0.99);
    zombie->rotA = (float)((double)zombie->rotA + (frand - frand) * frand * frand * (double)0.009999999776482582);
    xa = (float)sin((double)zombie->rot);
    ya = (float)cos((double)zombie->rot);

    if (zombie->entity.onGround && frand < 0.01) {
        zombie->entity.yd = 0.12F;
    }

    moveRelative(&zombie->entity, xa, ya, zombie->entity.onGround ? 0.02F : 0.005F);
    zombie->entity.yd = (float)((double)zombie->entity.yd - 0.005);
    entity_move(&zombie->entity, zombie->entity.xd, zombie->entity.yd, zombie->entity.zd);
    zombie->entity.xd *= 0.91F;
    zombie->entity.yd *= 0.98F;
    zombie->entity.zd *= 0.91F;

    if (zombie->entity.y > 100.0F) {
        resetPos(&zombie->entity);
    }

    if (zombie->entity.onGround) {
        zombie->entity.xd *= 0.8F;
        zombie->entity.zd *= 0.8F;
    }
}

void zombie_render(Zombie* zombie, float a) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures_load("char.png", GL_NEAREST));
    glPushMatrix();
    double time = (double)util_getTimeInNano() / 1.0E9 * 10.0 * (double)zombie->speed + (double)zombie->timeOffs;
    float size = 0.058333334F;
    float yy = (float)(-fabs(sin(time * 0.6662)) * 5.0 - 23.0);
    glTranslatef(zombie->entity.xo + (zombie->entity.x - zombie->entity.xo) * a, zombie->entity.yo + (zombie->entity.y - zombie->entity.yo) * a, zombie->entity.zo + (zombie->entity.z - zombie->entity.zo) * a);
    glScalef(1.0F, -1.0F, 1.0F);
    glScalef(size, size, size);
    glTranslatef(0.0F, yy, 0.0F);
    float c = 57.29578F;
    glRotatef(zombie->rot * c + 180.0F, 0.0F, 1.0F, 0.0F);
    zombie->head.yRot = (float)sin(time * 0.83) * 1.0F;
    zombie->head.xRot = (float)sin(time) * 0.8F;
    zombie->arm0.xRot = (float)sin(time * 0.6662 + M_PI) * 2.0F;
    zombie->arm0.zRot = (float)(sin(time * 0.2312) + 1.0) * 1.0F;
    zombie->arm1.xRot = (float)sin(time * 0.6662) * 2.0F;
    zombie->arm1.zRot = (float)(sin(time * 0.2812) - 1.0) * 1.0F;
    zombie->leg0.xRot = (float)sin(time * 0.6662) * 1.4F;
    zombie->leg1.xRot = (float)sin(time * 0.6662 + M_PI) * 1.4F;
    cube_render(&zombie->head);
    cube_render(&zombie->body);
    cube_render(&zombie->arm0);
    cube_render(&zombie->arm1);
    cube_render(&zombie->leg0);
    cube_render(&zombie->leg1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}