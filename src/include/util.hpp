#pragma once

#include "anchor.hpp"
#include "types.hpp"
#include "vector2.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3_ttf/SDL_ttf.h"

#define ERROR_RETURN(R, ...) { std::print(stderr, __VA_ARGS__); return R; }

#define ERROR_LOG(...) { std::print(stderr, __VA_ARGS__); }

#define HANDLE_SDL_ERROR_RETURN(F, S) { if (!(F)) {SDL_Log(S, SDL_GetError()); return false;} }

#define HANDLE_SDL_ERROR(F, S) { if (!(F)) {SDL_Log(S, SDL_GetError());} }

#define OPEN_MENU(menu) Menu::open(menu); mInMenu=true; mCurrentMenu=menu;
#define CLOSE_MENU(menu) Menu::close(menu); mInMenu=false; mCurrentMenu="";

void U_DrawRendererText( TTF_Text *text, float x, float y, PositionAnchor anchor = ANCHOR_TOP_LEFT );

Vector2 U_GetTextSize(TTF_Text * text);

void U_AnchorFRect( int anchor, SDL_FRect *rect );

bool U_PointInRectFloat( float x, float y, const SDL_FRect *r );