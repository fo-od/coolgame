#pragma once

#include "engine/ui/element.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <functional>

class Button final : public Element
{
    bool mHovered = false;
    TTF_Text *mText;
    std::function< void() > mOnClick;

    public:
        Button( TTF_TextEngine *textEngine, TTF_Font *font, float xOffset, float yOffset, Anchor anchor, Anchor origin,
                const char *text, const std::function< void() >& onClick );

        ~Button() override;

        void draw( SDL_Renderer *renderer ) const override;

        void handle_input( const SDL_MouseMotionEvent& motion, const SDL_MouseButtonEvent& button ) override;
};