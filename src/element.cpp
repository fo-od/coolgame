#include "element.hpp"

#include "util.hpp"

Element::Element( const float xOffset, const float yOffset, const float w, const float h,
                  const int anchor ) : mRect(SDL_FRect(0, 0, w, h)), mAnchor(anchor), mXOffset(xOffset),
                                       mYOffset(yOffset) {}

void Element::draw( SDL_Renderer *renderer ) const
{
    HANDLE_SDL_ERROR(SDL_RenderFillRect(renderer, &mRect), "Failed to draw Element: %s")
}

void Element::update_position( const int canvasWidth, const int canvasHeight )
{
    if (mAnchor != ANCHOR_NONE) {
        if (mAnchor == ANCHOR_CENTER) {
            mRect.x = canvasWidth / 2 + mXOffset;
            mRect.y = canvasHeight / 2 - mYOffset;
        } else {
            if (mAnchor & ANCHOR_CENTER) {
                mRect.y = canvasHeight / 2 - mYOffset;
            }
            if (mAnchor & ANCHOR_BOTTOM) {
                mRect.y = canvasHeight - mRect.h - mYOffset;
            }
        }
    }
}