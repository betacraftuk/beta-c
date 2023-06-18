#define SDL_MAIN_HANDLED

#include "Util.h"
#include "character/Zombie.h"
#include "level/Chunk.h"
#include "level/Level.h"
#include "level/LevelRenderer.h"
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

#define COL 920330

int width = 1024;
int height = 768;
float fogColor[4];
Timer timer;
Level level;
Player player;
Zombie zombies[100];
int viewportBuffer[16];
int selectBuffer[2000];
HitResult hitResult;
int isHitNull = 1;

void init() {
    fogColor[0] = (float)((COL >> 16 & 255) / 255.0F);
    fogColor[1] = (float)((COL >> 8 & 255) / 255.0F);
    fogColor[2] = (float)((COL & 255) / 255.0F);
    fogColor[3] = 1.0F;
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5, 0.8, 1.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    createTimer(&timer, 60.0f);
    level_create(&level, 256, 256, 64);
    player_create(&player, &level);

    for (int i = 0; i < 100; i++) {
        zombie_create(&zombies[i], &level, 128.0F, 0.0F, 128.0F);
    }
}

void tick() {
    for (int i = 0; i < 100; i++) {
        zombie_tick(&zombies[i]);
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

void render(SDL_Event* e, SDL_Window* window) {
    if (e->type == SDL_MOUSEMOTION) {
        entity_turn(&player.entity, e->motion.xrel, -e->motion.yrel);
    }

    pick(timer.a);

    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_RIGHT && !isHitNull) {
        level_setTile(&level, hitResult.x, hitResult.y, hitResult.z, 0);
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

        level_setTile(&level, x, y, z, 1);
    }

    if ((e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN)) {
        level_save(&level);
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    setupCamera(timer.a);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.2F);
    glFogfv(GL_FOG_COLOR, fogColor);
    glDisable(GL_FOG);
    levelrenderer_render(&level, &player, 0);

    for (int i = 0; i < 100; i++) {
        zombie_render(&zombies[i], timer.a);
    }

    glEnable(GL_FOG);
    levelrenderer_render(&level, &player, 1);
    glDisable(GL_TEXTURE_2D);

    if (!isHitNull) {
        levelrenderer_renderHit(&level.renderer, &hitResult);
    }

    glDisable(GL_FOG);
    SDL_GL_SwapWindow(window);
}

int main() { 
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("%s\n", "Failed to initialize SDL");
        exit(0);
	}

    SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, flags);
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
                resetPos(&player.entity);
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

    return 0;
}
