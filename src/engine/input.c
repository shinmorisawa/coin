#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include "engine/input.h"
#include "engine/render.h"

extern bool running;

static InputState input;

void engine_input(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT: running = false; break;
            case SDL_EVENT_MOUSE_MOTION: 
                input.mouse_x = e.motion.x;
                input.mouse_y = e.motion.y;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (e.button.button == SDL_BUTTON_LEFT) input.left_mouse_pressed = true;
                if (e.button.button == SDL_BUTTON_RIGHT) input.right_mouse_pressed = true;
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (e.button.button == SDL_BUTTON_LEFT) input.left_mouse_pressed = false;
                if (e.button.button == SDL_BUTTON_RIGHT) input.right_mouse_pressed = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (e.key.scancode == SDL_SCANCODE_ESCAPE) { running = false; break; }
                input.keys[e.key.scancode] = true;
                break;
            case SDL_EVENT_KEY_UP:
                input.keys[e.key.scancode] = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                int w = e.window.data1;
                int h = e.window.data2;
                printf("[engine/input.resize] %dx%d\n", w, h);
                render_resize(w, h);
                break;
        }
    }
}

InputState* engine_input_get(void) {
    return &input;
}
