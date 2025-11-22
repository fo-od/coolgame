#include "util.hpp"

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

void U_DrawRendererText( TTF_Text *text, float x, float y, const int anchor )
{
    if (anchor != ANCHOR_TOP_LEFT) {
        int w, h = -1;
        HANDLE_SDL_ERROR(TTF_GetTextSize(text, &w, &h), "Failed to render text: %s");

        switch (anchor) {
            case ANCHOR_TOP_MIDDLE:
                x -= w / 2;
                break;
            case ANCHOR_TOP_RIGHT:
                x -= w;
                break;
            case ANCHOR_MIDDLE_LEFT:
                y -= h / 2;
                break;
            case ANCHOR_MIDDLE_MIDDLE:
                x -= w / 2;
                y -= h / 2;
                break;
            case ANCHOR_MIDDLE_RIGHT:
                x -= w;
                y -= h / 2;
                break;
            case ANCHOR_BOTTOM_LEFT:
                y -= h;
                break;
            case ANCHOR_BOTTOM_MIDDLE:
                x -= w / 2;
                y -= h;
                break;
            case ANCHOR_BOTTOM_RIGHT:
                x -= w;
                y -= h;
                break;
            default:
                SDL_Log("Invalid anchor given!");
                break;
        }
    }
    TTF_DrawRendererText(text, x, y);
}

Vector2 U_GetTextSize( TTF_Text *text )
{
    int w, h;
    TTF_GetTextSize(text, &w, &h);
    return Vector2{w, h};
}

void U_AnchorFRect( const int anchor, SDL_FRect *rect )
{
    if (anchor != ANCHOR_TOP_LEFT) {
        switch (anchor) {
            case ANCHOR_TOP_MIDDLE:
                rect->x -= rect->w / 2;
                break;
            case ANCHOR_TOP_RIGHT:
                rect->x -= rect->w;
                break;
            case ANCHOR_MIDDLE_LEFT:
                rect->y -= rect->h / 2;
                break;
            case ANCHOR_MIDDLE_MIDDLE:
                rect->x -= rect->w / 2;
                rect->y -= rect->h / 2;
                break;
            case ANCHOR_MIDDLE_RIGHT:
                rect->x -= rect->w;
                rect->y -= rect->h / 2;
                break;
            case ANCHOR_BOTTOM_LEFT:
                rect->y -= rect->h;
                break;
            case ANCHOR_BOTTOM_MIDDLE:
                rect->x -= rect->w / 2;
                rect->y -= rect->h;
                break;
            case ANCHOR_BOTTOM_RIGHT:
                rect->x -= rect->w;
                rect->y -= rect->h;
                break;
            default:
                SDL_Log("Invalid anchor given!");
                break;
        }
    }
}

bool U_PointInRectFloat( const float x, const float y, const SDL_FRect *r )
{
    return ((x >= r->x) && (x <= (r->x + r->w)) &&
            (y >= r->y) && (y <= (r->y + r->h)));
}
