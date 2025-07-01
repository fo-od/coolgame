#include "io.hpp"

#include "globals.hpp"
#include "SDL3/SDL_render.h"

void update_mouse( SDL_Event *e )
{
    if ( e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type ==
         SDL_EVENT_MOUSE_BUTTON_UP ) {
        SDL_ConvertEventToRenderCoordinates(renderer, e);
        mouse.pos.x = e->motion.x;
        mouse.pos.y = e->motion.y;
        mouse.buttons = e->motion.state;
    }
}

void update_keyboard()
{
    keyboardState = SDL_GetKeyboardState(nullptr);
}
