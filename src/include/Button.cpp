#include "Button.hpp"

#include <functional>
#include <string>

#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_render.h"
#include "util.cpp"

Button::Button(const std::string &text, const float x, const float y, const std::function<void()> &onClick) {
    this->text = TTF_CreateText(textEngine, font, text.c_str(), 0);
    this->onClick = onClick;

    int textWidth, textHeight;
    TTF_GetTextSize(this->text, &textWidth, &textHeight);
    this->rect = {x, y, static_cast<float>(textWidth) + 10, static_cast<float>(textHeight) + 10};
}

void Button::update() const {
    if (SDL_PointInRectFloat(&mouse->pos, &rect)) {
        U_SetRenderDrawColor(COLOR_WHITE);
        SDL_RenderFillRect(renderer, &rect);
        if (mouse->buttons & SDL_BUTTON_LEFT) {
            this->onClick();
        }
    } else {
        SDL_RenderRect(renderer, &rect);
    }
}
