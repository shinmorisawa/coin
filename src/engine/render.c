#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "engine/render.h"
#include "engine/ecs.h"
#include "engine/ecs/storage.h"
#include "glad/glad.h"

extern SDL_Window* window;

static float view[16] = {
    1.0f, 0, 0, 0,
    0, 1.0f, 0, 0,
    0, 0, 1.0f, 0,
    0, 0, -3.0f, 1.0f
};

float aspect = 4.0f / 3.0f;

void engine_render_init(void) {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void engine_render(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_view_matrix();
    render_system();

    SDL_GL_SwapWindow(window);
}

void render_view_matrix(void) {
    float cx = cosf(-camera.rx);
    float sx = sinf(-camera.rx);
    float cy = cosf(-camera.ry);
    float sy = sinf(-camera.ry);
    float cz = cosf(-camera.rz);
    float sz = sinf(-camera.rz);

    float Rx[16] = {
        1, 0, 0, 0,
        0, cx, sx, 0,
        0, -sx, cx, 0,
        0, 0, 0, 1
    };

    float Ry[16] = {
        cy, 0, -sy, 0,
        0, 1, 0, 0,
        sy, 0, cy, 0,
        0, 0, 0, 1
    };

    float Rz[16] = {
        cz, sz, 0, 0,
        -sz, cz, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    float R[16];
    float Rxy[16];
    
    mat4_mul(Rxy, Rx, Ry);
    mat4_mul(R, Rxy, Rz);

    float T[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -camera.x, -camera.y, -camera.z, 1
    };

    mat4_mul(view, R, T);
}

void renderer_draw_mesh(Transform t, Mesh m) {
    /* scale matrix */
    float S[16] = {
        t.sx, 0, 0, 0,
        0, t.sy, 0, 0,
        0, 0, t.sz, 0,
        0, 0, 0, 1.0f
    };

    /* rotation matrices */
    float sx = sinf(t.rx);
    float sy = sinf(t.ry);
    float sz = sinf(t.rz);

    float cx = cosf(t.rx);
    float cy = cosf(t.ry);
    float cz = cosf(t.rz);

    float Rx[16] = {
        1, 0, 0, 0,
        0, cx, sx, 0,
        0, -sx, cx, 0,
        0, 0, 0, 1
    };

    float Ry[16] = {
        cy, 0, -sy, 0,
        0, 1, 0, 0,
        sy, 0, cy, 0,
        0, 0, 0, 1
    };

    float Rz[16] = {
        cz, sz, 0, 0,
        -sz, cz, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    float Rxy[16];
    float R[16];

    mat4_mul(Rxy, Ry, Rx);
    mat4_mul(R, Rz, Rxy);

    /* translation matrix */
    float T[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        t.x, t.y, t.z, 1
    };

    float model[16];

    mat4_mul(model, T, R);
    mat4_mul(model, model, S);

    glUseProgram(m.shader.shader);

    glUniformMatrix4fv(m.shader.model_loc, 1, GL_FALSE, model);
    glUniformMatrix4fv(m.shader.view_loc, 1, GL_FALSE, view);
    glUniform3f(m.shader.camera_position_loc, camera.x, camera.y, camera.z);

    glBindVertexArray(m.VAO);
    glDrawArrays(GL_TRIANGLES, 0, m.vertex_count);
}

Mesh mesh_create(const Vertex* vertices, size_t vertex_size, int vertex_count) {
    Mesh m = {0};

    glGenVertexArrays(1, &m.VAO);
    glGenBuffers(1, &m.VBO);

    glBindVertexArray(m.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertices, GL_STATIC_DRAW);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    m.vertex_count = vertex_count;

    return m;
}

void render_resize(int w, int h) {
    glViewport(0, 0, w, h);
    aspect = (float)w / (float)h;

    float fov = 60.0f * (3.1415926f / 180.0f);
    float near = 0.1f;
    float far = 100.0f;
    float f = 1.0f / tanf(fov/2);

    float projection[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near)/(near - far), -1,
        0, 0, (2*far*near)/(near - far), 0
    };

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (has_mesh[i] != true) continue;
        glUseProgram(meshes[i].shader.shader);
        glUniformMatrix4fv(meshes[i].shader.projection_loc, 1, GL_FALSE, projection);
    }
}
