#pragma once

#include "types.hpp"
#include "vector2.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3_ttf/SDL_ttf.h"

#define ERROR_RETURN(R, ...) { std::print(stderr, __VA_ARGS__); return R; }

#define ERROR_LOG(...) { std::print(stderr, __VA_ARGS__); }

#define HANDLE_SDL_ERROR_RETURN(F, S) { if (!(F)) {SDL_Log(S, SDL_GetError()); return false;} }

#define HANDLE_SDL_ERROR(F, S) { if (!F) {SDL_Log(S, SDL_GetError());} }

void U_SetRenderDrawColor( SDL_Color color );

void U_SetRenderDrawColor( u8 r = 0, u8 g = 0, u8 b = 0, u8 a = 255 );

void U_DrawRendererText( TTF_Text *text, int anchor, float x, float y );

Vector2 U_GetTextSize( TTF_Text *text );

bool U_SetTextColor( TTF_Text *text, SDL_Color color );

void U_AnchorFRect( int anchor, SDL_FRect *rect );

void U_RenderRect( const SDL_FRect *rect, u8 r = 0, u8 g = 0, u8 b = 0, u8 a = 255 );

void U_RenderRect( const SDL_FRect *rect, SDL_Color color );

void U_RenderLine( const Vector2 &a, const Vector2 &b );
