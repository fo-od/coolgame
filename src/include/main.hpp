#pragma once
#include <SDL3/SDL_events.h>

bool init();

void handle_events(const SDL_Event *e);

void update();

void render();

void cleanup();