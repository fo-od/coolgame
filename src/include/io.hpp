#pragma once

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"

typedef struct
{
    SDL_FPoint pos;
    SDL_MouseButtonFlags buttons;
} Mouse;

inline Mouse mouse;
inline const bool *keyboardState;

void update_mouse( SDL_Event *e );

void update_keyboard();
