#pragma once

#include "util.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3_ttf/SDL_ttf.h"

class Element
{
    protected:
        SDL_FRect mRect{};
        int mAnchor;

    public:
        explicit Element( SDL_FRect rect, int anchor = ANCHOR_NONE );

        Element( float x, float y, float w, float h, int anchor = ANCHOR_NONE );

        Element( const Element &other ) = default;

        virtual ~Element() = default;

        virtual void draw( SDL_Renderer *renderer ) const;

        virtual void update( const SDL_MouseMotionEvent &motion, const SDL_MouseButtonEvent &button );
};
