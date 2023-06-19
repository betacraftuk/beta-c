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
    zombiemodel_create(&zombie->zombieModel);
    entity_create(&zombie->entity, level);
    entity_setPos(&zombie->entity, x, y, z);
    zombie->timeOffs = (float)frand * 1239813.0F;
    zombie->rotA = (float)(frand + 1.0) * 0.01F;
    zombie->rot = (float)(frand * M_PI * 2.0);
    zombie->speed = 1.0F;
}

void zombie_tick(Zombie* zombie) {
    zombie->entity.xo = zombie->entity.x;
    zombie->entity.yo = zombie->entity.y;
    zombie->entity.zo = zombie->entity.z;
    float xa = 0.0F;
    float ya = 0.0F;

    if (zombie->entity.y < -100.0F) {
        zombie->entity.removed = 1;
    }

    zombie->rot += zombie->rotA;
    zombie->rotA = (float)((double)zombie->rotA * 0.99);
    zombie->rotA = (float)((double)zombie->rotA + (frand - frand) * frand * frand * (double)0.08F);
    xa = (float)sin((double)zombie->rot);
    ya = (float)cos((double)zombie->rot);

    if (zombie->entity.onGround && frand < 0.08) {
        zombie->entity.yd = 0.5F;
    }

    entity_moveRelative(&zombie->entity, xa, ya, zombie->entity.onGround ? 0.1F : 0.02F);
    zombie->entity.yd = (float)((double)zombie->entity.yd - 0.08);
    entity_move(&zombie->entity, zombie->entity.xd, zombie->entity.yd, zombie->entity.zd);
    zombie->entity.xd *= 0.91F;
    zombie->entity.yd *= 0.98F;
    zombie->entity.zd *= 0.91F;

    if (zombie->entity.onGround) {
        zombie->entity.xd *= 0.7F;
        zombie->entity.zd *= 0.7F;
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
    zombiemodel_render(&zombie->zombieModel, time);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}