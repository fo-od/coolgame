#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

#define ERROR_RETURN(R, ...) { fprintf(stderr, __VA_ARGS__); return R; }

#define ERROR_LOG(...) { fprintf(stderr, __VA_ARGS__); }

void U_SetRenderDrawColor( SDL_Color color );

void U_DrawRendererText( TTF_Text *text, int anchor, float x, float y );

bool U_SetTextColor( TTF_Text *text, SDL_Color color );

void U_AnchorFRect( int anchor, SDL_FRect *rect );
