#pragma once

typedef struct GameContext {
    int window_width, window_height;
    char* window_name;
    void* (*game_logic)(void* arg);
} GameContext;

typedef void (*InitCallback)(void);

void engine_register_init_callback(InitCallback cb);
void engine_init(GameContext game);
