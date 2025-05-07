#include "util.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "constants.hpp"
#include "globals.hpp"

void update_mouse( const SDL_Event *e )
{
    if ( e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
         e->type == SDL_EVENT_MOUSE_BUTTON_UP ) {
        mouse.buttons = e->motion.state;
        mouse.pos.x = e->motion.x;
        mouse.pos.y = e->motion.y;
    }
}

void update_keyboard()
{
    keyboardState = SDL_GetKeyboardState(NULL);
}


void U_SetRenderDrawColor( const SDL_Color color )
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

bool U_SetTextColor( TTF_Text *text, const SDL_Color color )
{
    return TTF_SetTextColor(text, color.r, color.g, color.b, color.a);
}

void U_DrawRendererText( TTF_Text *text, const int anchor, float x, float y )
{
    int w, h = -1;
    TTF_GetTextSize(text, &w, &h);

    switch ( anchor ) {
        case ANCHOR_TOP_LEFT:
            // text anchor pos is already top left
            break;
        case ANCHOR_TOP_MIDDLE:
            x -= static_cast< float >(w) / 2;
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
