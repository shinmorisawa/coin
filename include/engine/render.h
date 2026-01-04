#pragma once

#include "engine/ecs.h"
#include <SDL3/SDL.h>

#define PI 3.1415926f

/* render.c */
void engine_render_init(void);
void engine_render(void);
void renderer_draw_mesh(Transform t, Mesh m);
Mesh mesh_create(const float* vertices, size_t vertex_size, int vertex_count);

/* render_math.c */
void mat4_mul(float out[16], const float a[16], const float b[16]);

/* ecs/systems/render.c */
void render_system(void);
