#pragma once

#include "body.hpp"
#include "SDL3/SDL_events.h"

bool init_sdl();

bool init_game();

void handle_events( SDL_Event *e );

void update( Body *player );

void render();

void cleanup();
