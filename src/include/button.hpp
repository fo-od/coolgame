#pragma once

#include <functional>

#include "element.hpp"
#include <SDL3_ttf/SDL_ttf.h>

class Button final : public Element
{
    TTF_Text *mText;
    std::function<void()> mOnClick;
    bool mHovered = false;

    public:
        Button( TTF_TextEngine *textEngine, TTF_Font *font, float x, float y, int anchor, const char *text,
                const std::function<void()> &onClick );

        ~Button() override;

        void draw( SDL_Renderer *renderer ) const override;

        void update( const SDL_MouseMotionEvent &motion, const SDL_MouseButtonEvent &button ) override;
};