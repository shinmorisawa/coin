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

static const GLchar* emission_vsh_src =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aNormal;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "out vec3 vNormal;\n"
    "out vec3 vWorldPos;\n"
    "void main() {\n"
    "   mat3 normal_matrix = transpose(inverse(mat3(model)));\n"
    "   vNormal = normalize(normal_matrix * aNormal);\n"
    "   vWorldPos = vec3(model * vec4(aPos, 1.0));\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\0";

static const GLchar* emission_fsh_src =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 vNormal;\n"
    "in vec3 vWorldPos;\n"
    "void main() {\n"
    "   float ambient = 0.1;\n"
    "   vec3 base = vec3(1.0, 0.2, 0.6);\n"
    "   vec3 light_position = vec3(2.0, 3.0, 2.0);\n"
    "   vec3 n = normalize(vNormal);\n"
    "   vec3 l = normalize(light_position - vWorldPos);\n"
    "   float d = max(dot(n, l), 0.0);\n"
    "   vec3 color = base * (ambient + d);\n"
    "   FragColor = vec4(color, 1.0);\n"
    "}\0";

unsigned int emission_shader;

static float view[16] = {
    1.0f, 0, 0, 0,
    0, 1.0f, 0, 0,
    0, 0, 1.0f, 0,
    0, 0, -3.0f, 1.0f
};

void engine_render_init(void) {
    /* compile shaders */
    unsigned int vsh, fsh;
    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vsh, 1, &emission_vsh_src, NULL);
    glCompileShader(vsh);

    glShaderSource(fsh, 1, &emission_fsh_src, NULL);
    glCompileShader(fsh);

    emission_shader = glCreateProgram();
    glAttachShader(emission_shader, vsh);
    glAttachShader(emission_shader, fsh);
    glLinkProgram(emission_shader);

    int success;
    char infoLog[512];

    glGetShaderiv(vsh, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vsh, 512, NULL, infoLog);
        printf("vertex shader failed: %s", infoLog);
    }

    glGetShaderiv(fsh, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fsh, 512, NULL, infoLog);
        printf("fragment shader failed: %s", infoLog);
    }

    glGetProgramiv(emission_shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(emission_shader, 512, NULL, infoLog);
        printf("shader program linking failed: %s", infoLog);
    }

    glDeleteShader(vsh);
    glDeleteShader(fsh);

    /* set matrices up */
    float fov = 60.0f * (3.1415926f / 180.0f);
    float aspect = 4.0f / 3.0f;
    float near = 0.1f;
    float far = 100.0f;
    float f = 1.0f / tanf(fov/2);

    float projection[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near)/(near - far), -1,
        0, 0, (2*far*near)/(near - far), 0
    };

    float model[16] = {
        1.0f, 0, 0, 0,
        0, 1.0f, 0, 0,
        0, 0, 1.0f, 0,
        0, 0, 0, 1.0f
    };

    int model_loc = glGetUniformLocation(emission_shader, "model");
    int view_loc = glGetUniformLocation(emission_shader, "view");
    int projection_loc = glGetUniformLocation(emission_shader, "projection");

    glUseProgram(emission_shader);
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection);

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

    int model_loc = glGetUniformLocation(m.shader.shader, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

    int camera_position_loc = glGetUniformLocation(m.shader.shader, "camera");
    glUniform3f(camera_position_loc, camera.x, camera.y, camera.z);

    int view_loc = glGetUniformLocation(m.shader.shader, "view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, view);

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
