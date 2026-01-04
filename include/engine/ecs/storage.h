#pragma once

#include <stdbool.h>
#include "engine/ecs.h"

extern Transform transforms[MAX_ENTITIES];
extern bool has_transform[MAX_ENTITIES];

extern Mesh meshes[MAX_ENTITIES];
extern bool has_mesh[MAX_ENTITIES];

extern Camera camera;
