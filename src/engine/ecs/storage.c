#include <stdbool.h>
#include "engine/ecs.h"

Transform transforms[MAX_ENTITIES];
bool has_transform[MAX_ENTITIES];

Mesh meshes[MAX_ENTITIES];
bool has_mesh[MAX_ENTITIES];

Camera camera;
