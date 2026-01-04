#pragma once

#include <SDL3/SDL.h>

typedef struct InputState {
    bool left_mouse_pressed;
    bool right_mouse_pressed;
    bool keys[256];
    int mouse_x, mouse_y;
} InputState;

void engine_input();
InputState* engine_input_get();
