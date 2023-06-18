#include "Particle.h"

#include "../Util.h"
#include <math.h>

void particle_create(Particle* particle, Level* level, float x, float y, float z, float xa, float ya, float za, int tex) {
    entity_create(&particle->entity, level);
    particle->tex = tex;
    entity_setSize(&particle->entity, 0.2F, 0.2F);
    particle->entity.heightOffset = particle->entity.bbHeight / 2.0F;
    entity_setPos(&particle->entity, x, y, z);
    particle->xd = xa + (float)(frand * 2.0F - 1.0F) * 0.4F;
    particle->yd = ya + (float)(frand * 2.0F - 1.0F) * 0.4F;
    particle->zd = za + (float)(frand * 2.0F - 1.0F) * 0.4F;
    float speed = (frand + frand + 1.0F) * 0.15F;
    float dd = (float)sqrt((double)(particle->xd * particle->xd + particle->yd * particle->yd + particle->zd * particle->zd));
    particle->xd = particle->xd / dd * speed * 0.7F;
    particle->yd = particle->yd / dd * speed;
    particle->zd = particle->zd / dd * speed * 0.7F;
    particle->uo = (float)frand * 3.0F;
    particle->vo = (float)frand * 3.0F;
}

void particle_tick(Particle* particle) {
    particle->entity.xo = particle->entity.x;
    particle->entity.yo = particle->entity.y;
    particle->entity.zo = particle->entity.z;

    if (frand < 0.1F) {
        particle->entity.removed = 1;
    }

    particle->yd = (float)((double)particle->yd - 0.06);
    entity_move(&particle->entity, particle->xd, particle->yd, particle->zd);
    particle->xd *= 0.98F;
    particle->yd *= 0.98F;
    particle->zd *= 0.98F;

    if (particle->entity.onGround) {
        particle->xd *= 0.7F;
        particle->zd *= 0.7F;
    }
}

void particle_render(Particle* particle, float a, float xa, float ya, float za) {
    float u0 = ((float)(particle->tex % 16) + particle->uo / 4.0F) / 16.0F;
    float u1 = u0 + 0.999F / 64.0F;
    float v0 = ((float)(particle->tex / 16) + particle->vo / 4.0F) / 16.0F;
    float v1 = v0 + 0.999F / 64.0F;
    float r = 0.1F;
    float x = particle->entity.xo + (particle->entity.x - particle->entity.xo) * a;
    float y = particle->entity.yo + (particle->entity.y - particle->entity.yo) * a;
    float z = particle->entity.zo + (particle->entity.z - particle->entity.zo) * a;
    tesselator_vertexUV(x - xa * r, y - ya * r, z - za * r, u0, v1);
    tesselator_vertexUV(x - xa * r, y + ya * r, z - za * r, u0, v0);
    tesselator_vertexUV(x + xa * r, y + ya * r, z + za * r, u1, v0);
    tesselator_vertexUV(x + xa * r, y - ya * r, z + za * r, u1, v1);
}
