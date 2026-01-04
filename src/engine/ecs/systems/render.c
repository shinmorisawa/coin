#include "engine/render.h"
#include "engine/ecs/storage.h"

void render_system(void) {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!has_transform[e] || !has_mesh[e]) continue;

        renderer_draw_mesh(
            transforms[e],
            meshes[e]
        );
    }
}
