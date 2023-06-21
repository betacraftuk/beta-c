#include "ParticleEngine.h"

#include "../renderer/Textures.h"

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
    if (pe->particles->size == 0) {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    int id = textures_load("terrain.png", 9728);
    glBindTexture(GL_TEXTURE_2D, id);
    float xa = -((float)cos((double)player->entity.yRot * M_PI / 180.0));
    float za = -((float)sin((double)player->entity.yRot * M_PI / 180.0));
    float xa2 = -za * (float)sin((double)player->entity.xRot * M_PI / 180.0);
    float za2 = xa * (float)sin((double)player->entity.xRot * M_PI / 180.0);
    float ya = (float)cos((double)player->entity.xRot * M_PI / 180.0);
    glColor4f(0.8F, 0.8F, 0.8F, 1.0F);
    tesselator_init();

    Node* cur = pe->particles->head;

    while (cur != NULL) {
        Particle* p = (Particle*)cur->data;

        if (entity_isLit(&p->entity) ^ layer == 1) {
            particle_render(p, a, xa, ya, za, xa2, za2);
        }

        cur = cur->next;
    }

    tesselator_flush();
    glDisable(GL_TEXTURE_2D);
}