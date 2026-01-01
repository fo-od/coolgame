#include "include/util/util.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

void U_DrawRendererText( TTF_Text *text, float x, float y, const Anchor anchor )
{
    int w = -1;
    int h = -1;
    HANDLE_SDL_ERROR(TTF_GetTextSize(text, &w, &h), "Failed to render text: %s");

    // if the anchor is only center (so that we can also have bottom and top center)
    if (anchor == Anchor::Center) {
        x -= w / 2;
        y -= h / 2;
    } else {
        if (anchor & Anchor::Center) {
            y -= h / 2;
        }
        if (anchor & Anchor::Right) {
            x -= w;
        }
        if (anchor & Anchor::Bottom) {
            y -= h;
        }
    }
    TTF_DrawRendererText(text, x, y);
}

Vector2 U_GetTextSize( TTF_Text *text )
{
    int w;
    int h;
    TTF_GetTextSize(text, &w, &h);
    return Vector2{w, h};
}

void U_AnchorFRect( SDL_FRect *rect, const Anchor anchor )
{
    // if the anchor is only center (so that we can also have bottom and top center)
    if (anchor == Anchor::Center) {
        rect->x -= rect->w / 2;
        rect->y -= rect->h / 2;
    } else {
        if (anchor & Anchor::Center) {
            rect->y -= rect->h / 2;
        }
        if (anchor & Anchor::Right) {
            rect->x -= rect->w;
        }
        if (anchor & Anchor::Bottom) {
            rect->y -= rect->h;
        }
    }
}

bool U_PointInRectFloat( const float x, const float y, const SDL_FRect *r )
{
    return ((x >= r->x) && (x <= (r->x + r->w)) &&
            (y >= r->y) && (y <= (r->y + r->h)));
}