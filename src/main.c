#include <SDL3/SDL_pixels.h>
#include <unistd.h>
#include <stdio.h>
#include "engine/init.h"
#include "engine/input.h"
#include "engine/update.h"
#include "engine/render.h"
#include "engine/ecs.h"
#include "engine/ecs/storage.h"

float vertices[] = {
    // positions       
   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,

   -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,

   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,

   -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
};

void cool_callback(InputState* input, float dt) {
    (void)dt;
    // fprintf(stdout, "[game/cool_callback] mx: %d my: %d\n", input->mouse_x, input->mouse_y);
}

float angle = 0;
float angular_speed = 60.0f;
Entity e = 1;

void rotation_callback(InputState* input, float dt) {
    (void)input;
    float rad = angle * (PI / 180.0f);
    transforms[e] = (Transform) {
        .x = 0,
        .y = 0,
        .z = 0,
        .rx = rad,
        .ry = rad,
        .rz = rad,
        .sx = 1,
        .sy = 1,
        .sz = 1,
    };

    angle += angular_speed * dt;
}

extern unsigned int emission_shader;

void mesh_callback(void) {
    Mesh m = mesh_create(vertices, sizeof(vertices), (sizeof(vertices) / sizeof(float)) / 3);
    Transform t = {
        .x = 0,
        .y = 0,
        .z = 0,

        .rx = 0,
        .ry = 0,
        .rz = 0,

        .sx = 1,
        .sy = 1,
        .sz = 1
    };

    m.shader.shader = emission_shader;
    meshes[e] = m;
    transforms[e] = t;
    has_mesh[e] = true;
    has_transform[e] = true;

    fprintf(stdout, "[game/mesh] loaded meshes\n");
}

int main(void) {
    GameContext ctx = {
        .window_width = 1200,
        .window_height = 800,
        .window_name = "opengl works",
    };

    engine_register_update_callback(cool_callback);
    engine_register_update_callback(rotation_callback);
    engine_register_init_callback(mesh_callback);

    engine_init(ctx);

    return 0;
}
