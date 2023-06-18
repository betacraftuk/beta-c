#include "ParticleEngine.h"

#include "../Textures.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <math.h>

void particleengine_create(ParticleEngine* pe, Level* level) {
    pe->level = level;
    pe->particles = util_linkedlist_create();
}

void particleengine_tick(ParticleEngine* pe) {
    Node* cur = pe->particles->head;
    Particle* p;

    while (cur != NULL) {
        Node* nextNode = cur->next;
        p = (Particle*)cur->data;
        particle_tick(p);

        if (p->entity.removed) {
            util_linkedlist_remove_node(pe->particles, cur);
        }

        cur = nextNode;
    }
}

void particleengine_render(ParticleEngine* pe, Player* player, float a, int layer) {
    glEnable(GL_TEXTURE_2D);
    int id = textures_load("terrain.png", 9728);
    glBindTexture(GL_TEXTURE_2D, id);
    float xa = -((float)cos((double)player->entity.yRot * M_PI / 180.0));
    float za = -((float)sin((double)player->entity.yRot * M_PI / 180.0));
    float ya = 1.0F;
    glColor4f(0.8F, 0.8F, 0.8F, 1.0F);
    tesselator_init();

    Node* cur = pe->particles->head;

    while (cur != NULL) {
        Particle* p = (Particle*)cur->data;

        if (entity_isLit(&p->entity) ^ layer == 1) {
            particle_render(p, a, xa, ya, za);
        }

        cur = cur->next;
    }

    tesselator_flush();
    glDisable(GL_TEXTURE_2D);
}