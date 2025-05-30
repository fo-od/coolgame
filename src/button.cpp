#include "button.hpp"

#include <functional>
#include <string>

#include "constants.hpp"
#include "globals.hpp"
#include "io.hpp"
#include "util.hpp"

#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_render.h"

Button::Button( const std::string &text, const int anchor, const float x, const float y,
                const std::function< void() > &onClick )
    : text(TTF_CreateText(textEngine, font, text.c_str(), 0)),
      onClick(onClick)
{
    const Vector2 textSize = U_GetTextSize(this->text);
    this->rect = {x, y, textSize.x + 10, textSize.y + 10};

    U_AnchorFRect(anchor, &this->rect);
}

Button::~Button()
{
    TTF_DestroyText(text);
}

void Button::update() const
{
    U_SetRenderDrawColor(COLOR_WHITE);

    // if hovered
    if ( SDL_PointInRectFloat(&mouse.pos, &rect) ) {
        // fill the button
        SDL_RenderFillRect(renderer, &rect);
        U_SetTextColor(text, COLOR_BLACK);

        // if button is clicked
        if ( mouse.buttons & SDL_BUTTON_LEFT ) {
            this->onClick();
        }
    } else {
        // if the button is not hovered
        SDL_RenderRect(renderer, &rect);
        U_SetTextColor(text, COLOR_WHITE);
    }

    TTF_DrawRendererText(text, rect.x + 5, rect.y + 5);
    U_SetRenderDrawColor(COLOR_BLACK);
}
