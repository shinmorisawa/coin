#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include "engine/init.h"
#include "engine/loop.h"
#include "engine/update.h"
#include "engine/render.h"
#include "glad/glad.h"

bool running = true;

SDL_Window* window = NULL;
SDL_GLContext ctx;

int init_callback_count = 0;
InitCallback init_callbacks[MAX_CALLBACKS];

void engine_register_init_callback(InitCallback cb) {
    if (init_callback_count < MAX_CALLBACKS) {
        init_callbacks[init_callback_count++] = cb;
    } else {}
}

void engine_init_callbacks(void) {
    for (int i = 0; i < init_callback_count; i++) {
        init_callbacks[i]();
    }
}

void engine_init(GameContext game) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "[engine/init.c init] init failed\n");
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

    window = SDL_CreateWindow(game.window_name, game.window_width, game.window_height, SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "[engine/init.c init] window failed\n");
        return;
    }

    /* load an opengl context */
    ctx = SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD");
        return;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    fprintf(stdout, "[engine/init.GL] w: %d h: %d\n", w, h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    SDL_GL_SetSwapInterval(0); // 1 for vsync, 0 for unlimited
    
    engine_render_init();
    engine_init_callbacks();
    engine_loop();

    SDL_GL_DestroyContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
