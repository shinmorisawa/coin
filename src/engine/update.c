#include <stdio.h>
#include <SDL3/SDL.h>
#include "engine/update.h"
#include "engine/input.h"

bool debug = false;

UpdateCallback update_callbacks[MAX_CALLBACKS];
int callback_count = 0;

void engine_register_update_callback(UpdateCallback cb) {
    if (callback_count < MAX_CALLBACKS) {
        update_callbacks[callback_count++] = cb;
    } else {
        fprintf(stderr, "[engine/update] MAX_CALLBACKS reached\n");
    }
}

void engine_update(InputState* input, float dt) {
    for (int i = 0; i < callback_count; i++) {
        update_callbacks[i](input, dt);
    }
}
