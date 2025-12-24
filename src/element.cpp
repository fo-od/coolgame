#include "element.hpp"

#include "util.hpp"

Element::Element( const SDL_FRect rect, const int anchor ) : mRect(rect), mAnchor(anchor) {}

Element::Element( const float x, const float y, const float w, const float h, const int anchor ) : Element(
    SDL_FRect(x, y, w, h), anchor) {}

void Element::draw( SDL_Renderer *renderer ) const
{
    HANDLE_SDL_ERROR(SDL_RenderFillRect(renderer, &mRect), "Failed to draw Element: %s")
}

void Element::update_position( const int canvasWidth, const int canvasHeight )
{
    if (mAnchor != ANCHOR_NONE) {
        if (mAnchor == ANCHOR_CENTER) {
            mRect.x = (canvasWidth / 2.0f) + mXOffset;
            mRect.y = (canvasHeight / 2.0f) - mYOffset;
        } else {
            if (mAnchor & ANCHOR_CENTER) {
                mRect.y = (canvasHeight / 2.0f) - mYOffset;
            }
            if (mAnchor & ANCHOR_BOTTOM) {
                mRect.y = canvasHeight - mRect.h - mYOffset;
            }
        }
    }
}