#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_scancode.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "engine/init.h"
#include "engine/input.h"
#include "engine/update.h"
#include "engine/render.h"
#include "engine/ecs.h"
#include "engine/ecs/storage.h"

Vertex vertices[] = {
    // back face (-z)
    {{-0.5f, -0.5f, -0.5f}, { 0,  0, -1}, {0, 0}},
    {{ 0.5f, -0.5f, -0.5f}, { 0,  0, -1}, {1, 0}},
    {{ 0.5f,  0.5f, -0.5f}, { 0,  0, -1}, {1, 1}},
    {{ 0.5f,  0.5f, -0.5f}, { 0,  0, -1}, {1, 1}},
    {{-0.5f,  0.5f, -0.5f}, { 0,  0, -1}, {0, 1}},
    {{-0.5f, -0.5f, -0.5f}, { 0,  0, -1}, {0, 0}},

    // front face (+z)
    {{-0.5f, -0.5f,  0.5f}, { 0,  0,  1}, {0, 0}},
    {{ 0.5f, -0.5f,  0.5f}, { 0,  0,  1}, {1, 0}},
    {{ 0.5f,  0.5f,  0.5f}, { 0,  0,  1}, {1, 1}},
    {{ 0.5f,  0.5f,  0.5f}, { 0,  0,  1}, {1, 1}},
    {{-0.5f,  0.5f,  0.5f}, { 0,  0,  1}, {0, 1}},
    {{-0.5f, -0.5f,  0.5f}, { 0,  0,  1}, {0, 0}},

    // left face (-x)
    {{-0.5f,  0.5f,  0.5f}, {-1,  0,  0}, {1, 0}},
    {{-0.5f,  0.5f, -0.5f}, {-1,  0,  0}, {1, 1}},
    {{-0.5f, -0.5f, -0.5f}, {-1,  0,  0}, {0, 1}},
    {{-0.5f, -0.5f, -0.5f}, {-1,  0,  0}, {0, 1}},
    {{-0.5f, -0.5f,  0.5f}, {-1,  0,  0}, {0, 0}},
    {{-0.5f,  0.5f,  0.5f}, {-1,  0,  0}, {1, 0}},

    // right face (+x)
    {{ 0.5f,  0.5f,  0.5f}, { 1,  0,  0}, {1, 0}},
    {{ 0.5f,  0.5f, -0.5f}, { 1,  0,  0}, {1, 1}},
    {{ 0.5f, -0.5f, -0.5f}, { 1,  0,  0}, {0, 1}},
    {{ 0.5f, -0.5f, -0.5f}, { 1,  0,  0}, {0, 1}},
    {{ 0.5f, -0.5f,  0.5f}, { 1,  0,  0}, {0, 0}},
    {{ 0.5f,  0.5f,  0.5f}, { 1,  0,  0}, {1, 0}},

    // bottom face (-y)
    {{-0.5f, -0.5f, -0.5f}, { 0, -1,  0}, {0, 1}},
    {{ 0.5f, -0.5f, -0.5f}, { 0, -1,  0}, {1, 1}},
    {{ 0.5f, -0.5f,  0.5f}, { 0, -1,  0}, {1, 0}},
    {{ 0.5f, -0.5f,  0.5f}, { 0, -1,  0}, {1, 0}},
    {{-0.5f, -0.5f,  0.5f}, { 0, -1,  0}, {0, 0}},
    {{-0.5f, -0.5f, -0.5f}, { 0, -1,  0}, {0, 1}},

    // top face (+y)
    {{-0.5f,  0.5f, -0.5f}, { 0,  1,  0}, {0, 1}},
    {{ 0.5f,  0.5f, -0.5f}, { 0,  1,  0}, {1, 1}},
    {{ 0.5f,  0.5f,  0.5f}, { 0,  1,  0}, {1, 0}},
    {{ 0.5f,  0.5f,  0.5f}, { 0,  1,  0}, {1, 0}},
    {{-0.5f,  0.5f,  0.5f}, { 0,  1,  0}, {0, 0}},
    {{-0.5f,  0.5f, -0.5f}, { 0,  1,  0}, {0, 1}},
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
    transforms[1] = (Transform) {
        .x = -0.8f,
        .y = 0,
        .z = 0,
        .rx = rad,
        .ry = rad,
        .rz = rad,
        .sx = 1,
        .sy = 1,
        .sz = 1,
    };

    transforms[2] = (Transform) {
        .x = 0.8f,
        .y = 0,
        .z = 0,
        .rx = -rad,
        .ry = -rad,
        .rz = -rad,
        .sx = 1,
        .sy = 1,
        .sz = 1,
    };

    angle += angular_speed * dt;
}

extern unsigned int emission_shader;

void mesh_callback(void) {
    Mesh m = mesh_create(vertices, sizeof(vertices), sizeof(vertices) / sizeof(Vertex));
    Transform t = {
        .x = -0.8f,
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
    meshes[1] = m;
    transforms[1] = t;
    has_mesh[1] = true;
    has_transform[1] = true;

    Mesh m2 = mesh_create(vertices, sizeof(vertices), sizeof(vertices) / sizeof(Vertex));
    Transform t2 = {
        .x = 0.8f,
        .y = 0,
        .z = 0,

        .rx = 0,
        .ry = 0,
        .rz = 0,

        .sx = 1,
        .sy = 1,
        .sz = 1
    };

    m2.shader.shader = emission_shader;
    meshes[2] = m2;
    transforms[2] = t2;
    has_mesh[2] = true;
    has_transform[2] = true;

    fprintf(stdout, "[game/mesh] loaded meshes\n");
}

void camera_callback(InputState* input, float dt) {
    float speed = 5.0f * dt;
    float r_speed = PI / 2;

    vec3 forward = {
        -sinf(camera.ry) * cosf(camera.rx),
        sinf(camera.rx),
        -cosf(camera.ry) * cosf(camera.rx)
    };

    vec3 right = {
        cosf(camera.ry),
        0.0f,
        -sinf(camera.ry)
    };

    if (input->keys[SDL_SCANCODE_W]) {
        camera.x += forward.x * speed;
        camera.y += forward.y * speed;
        camera.z += forward.z * speed;
    }
    
    if (input->keys[SDL_SCANCODE_S]) {
        camera.x -= forward.x * speed;
        camera.y -= forward.y * speed;
        camera.z -= forward.z * speed;
    }

    if (input->keys[SDL_SCANCODE_D]) {
        camera.x += right.x * speed;
        camera.y += right.y * speed;
        camera.z += right.z * speed;
    }

    if (input->keys[SDL_SCANCODE_A]) {
        camera.x -= right.x * speed;
        camera.y -= right.y * speed;
        camera.z -= right.z * speed;
    }

    /*
    if (input->keys[SDL_SCANCODE_E]) {
        camera.y += speed * dt;
    }

    if (input->keys[SDL_SCANCODE_Q]) {
        camera.y -= speed * dt;
    }
    */

    if (input->keys[SDL_SCANCODE_LEFT]) {
        camera.ry += r_speed * dt;
    }

    if (input->keys[SDL_SCANCODE_RIGHT]) {
        camera.ry -= r_speed * dt;
    }

    if (input->keys[SDL_SCANCODE_UP]) {
        camera.rx += r_speed * dt;
    }

    if (input->keys[SDL_SCANCODE_DOWN]) {
        camera.rx -= r_speed * dt;
    }
}

int main(void) {
    GameContext ctx = {
        .window_width = 1200,
        .window_height = 800,
        .window_name = "opengl works",
    };

    engine_register_update_callback(cool_callback);
    engine_register_update_callback(rotation_callback);
    engine_register_update_callback(camera_callback);
    engine_register_init_callback(mesh_callback);

    engine_init(ctx);

    return 0;
}
