#pragma once

#include "engine/ecs.h"

Shader shader_compile_source(const char* vsh, const char* fsh);
Shader shader_load(const char* vsh_path, const char* fsh_path);
Shader shader_load_base(const char* name);
