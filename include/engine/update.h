#pragma once

#include "engine/input.h"

#define MAX_CALLBACKS 32

typedef void (*UpdateCallback)(InputState* input, float dt);

void engine_register_update_callback(UpdateCallback cb);
void engine_update(InputState* input, float dt);
