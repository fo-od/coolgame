#include "element.hpp"

#include "util.hpp"

Element::Element( const SDL_FRect rect ) : mRect(rect) {}

Element::Element( const float x, const float y, const float w, const float h ) : Element(SDL_FRect{x, y, w, h}) {}


void Element::draw( SDL_Renderer *renderer ) const
{
    HANDLE_SDL_ERROR(SDL_RenderFillRect(renderer, &mRect), "bro it didnt work: %s")
}

void Element::update( const SDL_MouseMotionEvent &motion, const SDL_MouseButtonEvent &button ) {}