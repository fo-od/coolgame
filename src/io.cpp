#include "io.hpp"

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