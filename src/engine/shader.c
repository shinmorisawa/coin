#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "engine/ecs.h"
#include "engine/shader.h"
#include "glad/glad.h"

Shader shader_compile_source(const char* vsh, const char* fsh) {
    Shader s = {0};
    unsigned int vsh_c, fsh_c;
    vsh_c = glCreateShader(GL_VERTEX_SHADER);
    fsh_c = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vsh_c, 1, &vsh, NULL);
    glCompileShader(vsh_c);

    glShaderSource(fsh_c, 1, &fsh, NULL);
    glCompileShader(fsh_c);

    s.shader = glCreateProgram();
    glAttachShader(s.shader, vsh_c);
    glAttachShader(s.shader, fsh_c);
    glLinkProgram(s.shader);

    int success;
    char infoLog[512];

    glGetShaderiv(vsh_c, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vsh_c, 512, NULL, infoLog);
        printf("[engine/shader.vsh] compile failed: %s\n", infoLog);
    }

    glGetShaderiv(fsh_c, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fsh_c, 512, NULL, infoLog);
        printf("[engine/shader.fsh] compile failed: %s\n", infoLog);
    }

    glGetProgramiv(s.shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(s.shader, 512, NULL, infoLog);
        printf("shader program linking failed: %s", infoLog);
    }

    glDeleteShader(vsh_c);
    glDeleteShader(fsh_c);

    int model_loc = glGetUniformLocation(s.shader, "model");
    int view_loc = glGetUniformLocation(s.shader, "view");
    int projection_loc = glGetUniformLocation(s.shader, "projection");
    int camera_position_loc = glGetUniformLocation(s.shader, "camera_position");

    s.model_loc = model_loc;
    s.view_loc = view_loc;
    s.projection_loc = projection_loc;
    s.camera_position_loc = camera_position_loc;

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

    glUseProgram(s.shader);
    glUniformMatrix4fv(s.model_loc, 1, GL_FALSE, model);
    glUniformMatrix4fv(s.projection_loc, 1, GL_FALSE, projection);

    return s;
}

static char* ftos(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    if (!buffer) { fclose(f); return NULL; }

    size_t read_bytes = fread(buffer, 1, size, f);
    if (read_bytes != size) {
        free(buffer);
        fclose(f);
        return NULL;
    }

    buffer[size] = '\0';
    fclose(f);
    return buffer;
}

Shader shader_load(const char* vsh_path, const char* fsh_path) {
    char* vsh_src = ftos(vsh_path);
    char* fsh_src = ftos(fsh_path);

    if (!vsh_src || !fsh_src) {
        fprintf(stderr, "[engine/shader.load_sources] failed to load shader\n");
    }

    Shader s = shader_compile_source(vsh_src, fsh_src);

    free(vsh_src);
    free(fsh_src);

    return s;
}

Shader shader_load_base(const char* name) {
    char vsh[256], fsh[256];
    snprintf(vsh, 256, "shaders/%s/%s.vsh", name, name);
    snprintf(fsh, 256, "shaders/%s/%s.fsh", name, name);
    return shader_load(vsh, fsh);
}
