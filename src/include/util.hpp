#pragma once

#include "SDL3/SDL_rect.h"
#include "SDL3_ttf/SDL_ttf.h"

#define ERROR_RETURN(R, ...) { std::print(stderr, __VA_ARGS__); return R; }

#define ERROR_LOG(...) { std::print(stderr, __VA_ARGS__); }

#define HANDLE_SDL_ERROR_RETURN(F, S) { if (!F) {SDL_Log(S, SDL_GetError()); return false;} }

#define HANDLE_SDL_ERROR(F, S) { if (!F) {SDL_Log(S, SDL_GetError());} }

void U_SetRenderDrawColor( SDL_Color color );

void U_DrawRendererText( TTF_Text *text, int anchor, float x, float y );

bool U_SetTextColor( TTF_Text *text, SDL_Color color );

void U_AnchorFRect( int anchor, SDL_FRect *rect );
