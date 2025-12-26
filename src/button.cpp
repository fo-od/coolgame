#include "button.hpp"

#include <functional>

#include "util.hpp"

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>

Button::Button( TTF_TextEngine *textEngine, TTF_Font *font, const float xOffset, const float yOffset, const int anchor,
                const int origin, const char *text, const std::function<void()> &onClick )
    : Element(xOffset, yOffset, 0, 0, anchor, origin), mText(TTF_CreateText(textEngine, font, text, 0)),
      mOnClick(onClick)
{
    const Vector2 textSize = U_GetTextSize(this->mText);
    mRect = {xOffset, yOffset, textSize.x + 10, textSize.y + 10};
    U_AnchorFRect(&mRect, anchor);
}

Button::~Button()
{
    TTF_DestroyText(mText);
}

void Button::handle_input( const SDL_MouseMotionEvent &motion, const SDL_MouseButtonEvent &button )
{
    if (U_PointInRectFloat(motion.x, motion.y, &mRect)) {
        mHovered = true;

        if (button.type == SDL_EVENT_MOUSE_BUTTON_DOWN && button.button == SDL_BUTTON_LEFT) {
            this->mOnClick();
        }
    } else {
        mHovered = false;
    }
}

void Button::draw( SDL_Renderer *renderer ) const
{
    if (mHovered) {
        SDL_RenderFillRect(renderer, &mRect);
        TTF_SetTextColor(mText, 0, 0, 0, 255);
    } else {
        SDL_RenderRect(renderer, &mRect);
        TTF_SetTextColor(mText, 255, 255, 255, 255);
    }

    U_DrawRendererText(mText, mRect.x + 5, mRect.y + 5);
}
