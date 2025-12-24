#pragma once

#include "util.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

class Element
{
    protected:
        SDL_FRect mRect{};
        int mAnchor;
        float mXOffset = 0;
        float mYOffset = 0;

    public:
        Element( float xOffset, float yOffset, float w, float h, int anchor );

        Element( const Element &other ) = default;

        virtual ~Element() = default;

        virtual void draw( SDL_Renderer *renderer ) const;

        virtual void handle_input( const SDL_MouseMotionEvent &motion, const SDL_MouseButtonEvent &button ) = 0;

        void update_position( int canvasWidth, int canvasHeight );
};