#include <stdio.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include "engine/loop.h"
#include "engine/input.h"
#include "engine/update.h"
#include "engine/render.h"

extern bool running;

void engine_loop(void) {
    double dt = 0;
    Uint64 now = SDL_GetTicks();
    Uint64 lastFPS = SDL_GetTicksNS();
    Uint64 last = 0;
    long long counter = 0;
    int frames = 0;

    while (running) {
        Uint64 nowFPS = SDL_GetTicksNS();
        now = SDL_GetTicks();
        dt = (now - last) / 1000.0f;
        frames++;
        last = now;

        engine_input();
        InputState* i = engine_input_get();
        engine_update(i, dt, counter);
        engine_render();
        counter++;

        if (nowFPS - lastFPS >= 1000000000) {
            fprintf(stdout, "[engine/loop.FPS] %d fps\n", frames);
            frames = 0;
            lastFPS = nowFPS;
        }
    }
}
