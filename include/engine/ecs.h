#pragma once

#include <stdint.h>

#define MAX_ENTITIES 512

typedef struct {
    float x,y,z;
} vec3;

typedef struct Shader {
    unsigned int shader;
} Shader;

typedef uint32_t Entity;

/* rx-rz are in RADIANS, not degrees. if you need radians,
 * just use the provided deg_to_rad() function in render.h.
 */
typedef struct Transform {
    float x, y, z;
    float rx, ry, rz;
    float sx, sy, sz;
} Transform;

typedef struct Mesh {
    unsigned int VBO;
    unsigned int VAO;
    int vertex_count;
    Shader shader;
} Mesh;

typedef struct Vertex {
    float pos[3];
    float normal[3];
    float uv[2];
} Vertex;

typedef struct Camera {
    float x, y, z;
    float rx, ry, rz;
} Camera;
