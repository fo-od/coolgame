#include "engine/ui/element.hpp"

#include "util/util.hpp"
#include <stdexcept>

Element::Element( const float xOffset, const float yOffset, const float w, const float h,
                  const Anchor anchor, const Anchor origin ) : mRect(SDL_FRect(-w, -h, w, h)),
                                                               mAnchor(anchor),
                                                               mOrigin(origin),
                                                               mXOffset(xOffset),
                                                               mYOffset(yOffset)
{
    if ( origin == Anchor::None )
    {
        throw std::invalid_argument("Origin can't be ANCHOR_NONE");
    }
}

void Element::draw( SDL_Renderer *renderer ) const
{
    HANDLE_SDL_ERROR(SDL_RenderFillRect(renderer, &mRect), "Failed to draw Element: %s")
}

void Element::update_position( const float canvasWidth, const float canvasHeight )
{
    // Anchor handling
    if ( mAnchor != Anchor::None )
    {
        if ( mAnchor == Anchor::Center )
        {
            mRect.x = (canvasWidth / 2) + mXOffset;
            mRect.y = (canvasHeight / 2) - mYOffset;
        } else
        {
            if ( mAnchor& Anchor::Center )
            {
                if ( mAnchor& Anchor::Top

                     
                ||
                mAnchor& Anchor::Bottom
                )
                {
                    mRect.x = (canvasWidth / 2) + mXOffset;
                }
                else
                {
                    mRect.y = (canvasHeight / 2) - mYOffset;
                }
            }
            if ( mAnchor& Anchor::Bottom )
            {
                mRect.y = canvasHeight - mYOffset;
            }
            if ( mAnchor& Anchor::Right )
            {
                mRect.x = canvasWidth + mXOffset;
            }
        }
    }

    // Origin handling
    if ( mOrigin == Anchor::Center )
    {
        mRect.x -= mRect.w / 2;
        mRect.y -= mRect.h / 2;
    } else
    {
        if ( mOrigin& Anchor::Center )
        {
            if ( mOrigin& Anchor::Top

                 
            ||
            mOrigin& Anchor::Bottom
            )
            {
                mRect.x -= mRect.w / 2;
            }
            else
            {
                mRect.y -= mRect.h / 2;
            }
        }
        if ( mOrigin& Anchor::Bottom )
        {
            mRect.y -= mRect.h;
        }
        if ( mOrigin& Anchor::Right )
        {
            mRect.x += mRect.w;
        }
    }
}