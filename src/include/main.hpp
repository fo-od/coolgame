#pragma once
#include <SDL3/SDL_events.h>

#include "player.hpp"

bool init();

void handle_events( const SDL_Event *e );

void update( Player *player );

void render( const Player *player );

void cleanup();
