#include "util.hpp"

#include "constants.hpp"
#include "globals.hpp"

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

void U_SetRenderDrawColor( const SDL_Color color )
{
    HANDLE_SDL_ERROR(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a),
                     "Could not set the draw color: %s");
}

void U_SetRenderDrawColor( const u8 r, const u8 g, const u8 b, const u8 a )
{
    HANDLE_SDL_ERROR(SDL_SetRenderDrawColor(renderer, r, g, b, a),
                     "Could not set the draw color: %s");
}

bool U_SetTextColor( TTF_Text *text, const SDL_Color color )
{
    return TTF_SetTextColor(text, color.r, color.g, color.b, color.a);
}

void U_DrawRendererText( TTF_Text *text, const int anchor, float x, float y )
{
    int w, h = -1;
    if ( !TTF_GetTextSize(text, &w, &h) ) {}

    switch ( anchor ) {
        case ANCHOR_TOP_LEFT:
            // text anchor pos is already top left
            break;
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
    switch ( anchor ) {
        case ANCHOR_TOP_LEFT:
            // pos is already top left
            break;
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

void U_RenderRect( const SDL_FRect *rect, const u8 r, const u8 g, const u8 b, const u8 a )
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderRect(renderer, rect);
}

void U_RenderRect( const SDL_FRect *rect, const SDL_Color color )
{
    U_SetRenderDrawColor(color);
    SDL_RenderRect(renderer, rect);
}

void U_RenderLine( const Vector2 &a, const Vector2 &b )
{
    SDL_RenderLine(renderer, a.x, a.y, b.x, b.y);
}
