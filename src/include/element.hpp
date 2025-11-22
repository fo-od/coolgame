#pragma once

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3_ttf/SDL_ttf.h"

class Element
{
    protected:
        SDL_FRect mRect{};

    public:
        explicit Element( SDL_FRect rect );

        Element( float x, float y, float w, float h );

        Element( const Element &other ) = default;

        virtual ~Element() = default;

        virtual void draw( SDL_Renderer *renderer ) const;

        virtual void update( const SDL_MouseMotionEvent &motion, const SDL_MouseButtonEvent &button );
};