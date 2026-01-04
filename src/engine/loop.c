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
    float dt = 0;
    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;

    while (running) {
        now = SDL_GetTicks();
        dt = (now - last) / 1000.0f;
        last = now;

        engine_input();
        InputState* i = engine_input_get();
        engine_update(i, dt);
        engine_render();
    }
}
