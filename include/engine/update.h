#pragma once

#include "engine/input.h"

#define MAX_CALLBACKS 32

typedef void (*UpdateCallback)(InputState* input, double dt, long long counter);

void engine_register_update_callback(UpdateCallback cb);
void engine_update(InputState* input, double dt, long long counter);
