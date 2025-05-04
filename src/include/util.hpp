#pragma once

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct {
    SDL_FPoint pos;
    SDL_MouseButtonFlags buttons;
} Mouse;

void update_mouse(const SDL_Event *e);

void U_SetRenderDrawColor(SDL_Color color);

void U_DrawRendererText(TTF_Text *text, int anchor, float x, float y);

bool U_SetTextColor(TTF_Text *text, SDL_Color color);
