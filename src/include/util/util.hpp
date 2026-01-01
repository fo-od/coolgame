#pragma once

#include "util/types.hpp"
#include "engine/ui/anchor.hpp"
#include "util/vector2.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

#define ERROR_RETURN(R, ...) { std::print(stderr, __VA_ARGS__); return R; }

#define ERROR_LOG(...) { std::print(stderr, __VA_ARGS__); }

#define HANDLE_SDL_ERROR_RETURN(F, S) { if (!(F)) {SDL_Log(S, SDL_GetError()); return false;} }

#define HANDLE_SDL_ERROR(F, S) { if (!(F)) {SDL_Log(S, SDL_GetError());} }

#define OPEN_MENU(menu) {Menu::open(menu); mInMenu=true; mCurrentMenu=menu; Menu::update(mWidth, mHeight);}
#define CLOSE_MENU(menu) {if (!mCurrentMenu.empty()) {Menu::close(menu); mInMenu=false; mCurrentMenu.clear();}}

void U_DrawRendererText( TTF_Text *text, float x, float y, Anchor anchor = Anchor::Top | Anchor::Left );

Vector2 U_GetTextSize(TTF_Text * text);

void U_AnchorFRect( SDL_FRect *rect, Anchor anchor );

bool U_PointInRectFloat( float x, float y, const SDL_FRect *r );