#define SDL_MAIN_HANDLED

#include "Util.h"
#include "character/Zombie.h"
#include "level/Chunk.h"
#include "level/Level.h"
#include "level/LevelRenderer.h"
#include "level/Frustum.h"
#include "level/tile/Tile.h"
#include "particle/ParticleEngine.h"
#include "Textures.h"
#include "Timer.h"

#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define COL0 16710650
#define COL1 920330

void setupFog(int i);

int width = 1024;
int height = 768;
float fogColor0[4];
float fogColor1[4];
Timer timer;
Level level;
Player player;
int paintTexture = 1;
ParticleEngine particleEngine;
LinkedList* zombies;
int viewportBuffer[16];
int selectBuffer[2000];
HitResult hitResult;
int isHitNull = 1;
float lb[16];

void init() {
    fogColor0[0] = (float)((COL0 >> 16 & 255) / 255.0F);
    fogColor0[1] = (float)((COL0 >> 8 & 255) / 255.0F);
    fogColor0[2] = (float)((COL0 & 255) / 255.0F);
    fogColor0[3] = 1.0F;
    fogColor1[0] = (float)((COL1 >> 16 & 255) / 255.0F);
    fogColor1[1] = (float)((COL1 >> 8 & 255) / 255.0F);
    fogColor1[2] = (float)((COL1 & 255) / 255.0F);
    fogColor1[3] = 1.0F;
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5, 0.8, 1.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    createTimer(&timer, 20.0f);
    tesselator_create();
    tile_init();
    level_create(&level, 256, 256, 64);
    player_create(&player, &level);
    particleengine_create(&particleEngine, &level);
    zombies = util_linkedlist_create();

    for (int i = 0; i < 10; i++) {
        Zombie* z = malloc(sizeof(Zombie));
        zombie_create(z, &level, 128.0F, 0.0F, 128.0F);
        entity_resetPos(&z->entity);
        util_linkedlist_add(zombies, z);
    }
}

void tick() {
    level_tick(&level);
    particleengine_tick(&particleEngine);

    Node* cur = zombies->head;
    Zombie* z;

    while (cur != NULL) {
        Node* nextNode = cur->next;
        z = (Zombie*)cur->data;
        zombie_tick(z);

        if (z->entity.removed) {
            util_linkedlist_remove_node(zombies, cur);
        }

        cur = nextNode;
    }

    player_tick(&player);
}

void moveCameraToPlayer(float a) {
    glTranslatef(0.0F, 0.0F, -0.3F);
    glRotatef(player.entity.xRot, 1.0F, 0.0F, 0.0F);
    glRotatef(player.entity.yRot, 0.0F, 1.0F, 0.0F);
    float x = player.entity.xo + (player.entity.x - player.entity.xo) * a;
    float y = player.entity.yo + (player.entity.y - player.entity.yo) * a;
    float z = player.entity.zo + (player.entity.z - player.entity.zo) * a;
    glTranslatef(-x, -y, -z);
}

void setupCamera(float a) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)width / (double)height, 0.05, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(a);
}

void setupOrthoCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)width, (double)height, 0.0, 100.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0F, 0.0F, -200.0F);
}

void setupPickCamera(float a, double x, double y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glGetIntegerv(GL_VIEWPORT, viewportBuffer);
    gluPickMatrix(x, y, 5.0, 5.0, viewportBuffer);
    gluPerspective(70.0, (double)width / (double)height, 0.05, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraToPlayer(a);
}

void pick(float a) {
    int selectBufferPos = 0;
    glSelectBuffer(sizeof(selectBuffer), selectBuffer);
    glRenderMode(GL_SELECT);
    setupPickCamera(a, width / 2, height / 2);
    levelrenderer_renderPick(&level, &player);
    int hits = glRenderMode(GL_RENDER);
    long closest = 0L;
    int names[10];
    int hitNameCount = 0;

    for (int i = 0; i < hits; i++) {
        int nameCount = selectBuffer[selectBufferPos++];
        long minZ = selectBuffer[selectBufferPos++];
        selectBufferPos++;
        int j;

        if (minZ >= closest && i != 0) {
            for (j = 0; j < nameCount; j++) {
                selectBufferPos++;
            }
        } else {
            closest = minZ;
            hitNameCount = nameCount;

            for (int j = 0; j < nameCount; j++) {
                names[j] = selectBuffer[selectBufferPos++];
            }
        }
    }

    if (hitNameCount > 0) {
        hitresult_create(&hitResult, names[0], names[1], names[2], names[3], names[4]);
        isHitNull = 0;
    } else {
        isHitNull = 1;
    }
}

void drawGui() {
    glClear(GL_DEPTH_BUFFER_BIT);
    setupOrthoCamera();
    glPushMatrix();
    glTranslatef((float)(width - 48), 48.0F, 0.0F);
    glScalef(48.0F, 48.0F, 48.0F);
    glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
    glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
    glTranslatef(1.5F, -0.5F, -0.5F);
    glBindTexture(GL_TEXTURE_2D, textures_load("terrain.png", 9728));
    glEnable(GL_TEXTURE_2D);
    tesselator_init();
    tile_render(&tile_tiles[paintTexture], &level, 0, -2, 0, 0);
    tesselator_flush();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    int wc = width / 2;
    int hc = height / 2;
    glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
    tesselator_init();
    tesselator_vertex(wc + 1, hc - 8, 0.0F);
    tesselator_vertex(wc - 0, hc - 8, 0.0F);
    tesselator_vertex(wc - 0, hc + 9, 0.0F);
    tesselator_vertex(wc + 1, hc + 9, 0.0F);
    tesselator_vertex(wc + 9, hc - 0, 0.0F);
    tesselator_vertex(wc - 8, hc - 0, 0.0F);
    tesselator_vertex(wc - 8, hc + 1, 0.0F);
    tesselator_vertex(wc + 9, hc + 1, 0.0F);
    tesselator_flush();
}

void setBuffer(float a, float b, float c, float d) {
    lb[0] = a;
    lb[1] = b;
    lb[2] = c;
    lb[3] = d;
}

void render(SDL_Event* e, SDL_Window* window) {
    if (e->type == SDL_MOUSEMOTION) {
        entity_turn(&player.entity, e->motion.xrel, -e->motion.yrel);
    }

    pick(timer.a);

    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_RIGHT && !isHitNull) {
        Tile* frustum = &tile_tiles[level_getTile(&level, hitResult.x, hitResult.y, hitResult.z)];
        int i = level_setTile(&level, hitResult.x, hitResult.y, hitResult.z, 0);

        if (!frustum->isNull && i) {
            tile_destroy(frustum, &level, hitResult.x, hitResult.y, hitResult.z, &particleEngine);
        }
    }

    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT && !isHitNull) {
        int x = hitResult.x;
        int y = hitResult.y;
        int z = hitResult.z;

        if (hitResult.f == 0) {
            y--;
        }

        if (hitResult.f == 1) {
             y++;
        }

        if (hitResult.f == 2) {
             z--;
        }

        if (hitResult.f == 3) {
             z++;
        }

        if (hitResult.f == 4) {
             x--;
        }

        if (hitResult.f == 5) {
             x++;
        }

        level_setTile(&level, x, y, z, paintTexture);
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    setupCamera(timer.a);
    glEnable(GL_CULL_FACE);
    levelrenderer_updateDirtyChunks(&level, &player);
    setupFog(0);
    glEnable(GL_FOG);
    levelrenderer_render(&level, &player, 0);

    Zombie* z;
    Node* cur = zombies->head;

    while (cur != NULL) {
        z = cur->data;

        if (entity_isLit(&z->entity) && frustum_visibleAABB(&z->entity.bb)) {
            zombie_render(z, timer.a);
        }

        cur = cur->next;
    }

    particleengine_render(&particleEngine, &player, timer.a, 0);
    setupFog(1);
    levelrenderer_render(&level, &player, 1);

    cur = zombies->head;
    while (cur != NULL) {
        z = cur->data;

        if (entity_isLit(&z->entity) && frustum_visibleAABB(&z->entity.bb)) {
            zombie_render(z, timer.a);
        }

        cur = cur->next;
    }

    particleengine_render(&particleEngine, &player, timer.a, 1);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);

    if (!isHitNull) {
        levelrenderer_renderHit(&level.renderer, &hitResult);
    }

    drawGui();
    SDL_GL_SwapWindow(window);
}

void spawnZombie() {
    Zombie* z = malloc(sizeof(Zombie));
    zombie_create(z, &level, player.entity.x, player.entity.y, player.entity.z);
    util_linkedlist_add(zombies, z);
}

int main() { 
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("%s\n", "Failed to initialize SDL");
        exit(0);
	}

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    width = 1024;
    height = 768;

    SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetSwapInterval(0);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    init();

    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
    SDL_SetRelativeMouseMode(1);

    uint64_t lastTime = util_getTimeInMs();
    int frames = 0;

    while (1) {
        SDL_Event event;
        SDL_PollEvent(&event);

        if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || event.type == SDL_QUIT)
            break;

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
            case SDLK_w:
                player.keys.isW = event.type == SDL_KEYDOWN;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                player.keys.isA = event.type == SDL_KEYDOWN;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                player.keys.isS = event.type == SDL_KEYDOWN;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                player.keys.isD = event.type == SDL_KEYDOWN;
                break;
            case SDLK_SPACE:
                player.keys.isSpace = event.type == SDL_KEYDOWN;
                break;
            case SDLK_r:
                entity_resetPos(&player.entity);
                break;
            case SDLK_RETURN:
                level_save(&level);
                break;
            case SDLK_1:
                paintTexture = 1;
                break;
            case SDLK_2:
                paintTexture = 3;
                break;
            case SDLK_3:
                paintTexture = 4;
                break;
            case SDLK_4:
                paintTexture = 5;
                break;
            case SDLK_g:
                spawnZombie();
                break;
            default:
                break;
            }
        }

        timer_advance(&timer);

        for (int e = 0; e < timer.ticks; e++) {
            tick();
        }

        render(&event, window);
        frames++;

        while (util_getTimeInMs() >= lastTime + 1000) {
            printf("%d fps, %d\n", frames, chunk_updates);
          
            chunk_updates = 0;
            lastTime += 1000L;
            frames = 0;
        }
    }

    level_save(&level);

    SDL_GL_DeleteContext(glcontext);  
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(level.renderer.chunks);
    free(level.blocks);
    free(level.lightDepths);
    util_linkedlist_free(zombies);
    util_linkedlist_free(particleEngine.particles);

    return 0;
}

void setupFog(int i) {
    if (i == 0) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.001F);
        glFogfv(GL_FOG_COLOR, fogColor0);
        glDisable(GL_LIGHTING);
    } else if (i == 1) {
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.06F);
        glFogfv(GL_FOG_COLOR, fogColor1);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        float br = 0.6F;
        setBuffer(br, br, br, 1.0F);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lb);
    }
}
