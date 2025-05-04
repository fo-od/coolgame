#pragma once

#include <functional>
#include <string>
#include <SDL3/SDL_rect.h>
#include <SDL3_ttf/SDL_ttf.h>

class Button {
    TTF_Text *text;
    std::function<void()> onClick;
    SDL_FRect rect{};

public:
    Button(const std::string &text, float x, float y, const std::function<void()> &onClick);

    ~Button();

    void update() const;
};
