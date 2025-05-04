#include "Button.hpp"

#include <functional>
#include <string>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>

#include "constants.hpp"
#include "globals.hpp"
#include "util.hpp"

Button::Button(const std::string &text, const int anchor, const float x, const float y, const std::function<void()> &onClick) {
    this->text = TTF_CreateText(textEngine, font, text.c_str(), 0);
    this->onClick = onClick;

    int textWidth, textHeight;
    TTF_GetTextSize(this->text, &textWidth, &textHeight);
    this->rect = {x, y, static_cast<float>(textWidth) + 10, static_cast<float>(textHeight) + 10};

    U_AnchorFRect(anchor, &this->rect);
}

Button::~Button() {
    TTF_DestroyText(text);
}

void Button::update() const {
    U_SetRenderDrawColor(COLOR_WHITE);
    if (SDL_PointInRectFloat(&mouse.pos, &rect)) {
        SDL_RenderFillRect(renderer, &rect);
        U_SetTextColor(text, COLOR_BLACK);
        if (mouse.buttons & SDL_BUTTON_LEFT) {
            this->onClick();
        }
    } else {
        SDL_RenderRect(renderer, &rect);
        U_SetTextColor(text, COLOR_WHITE);
    }
    TTF_DrawRendererText(text, rect.x + 5, rect.y + 5);
    U_SetRenderDrawColor(COLOR_BLACK);
}
