#pragma once

#include "../assets/cozette.hpp"

#include "SDL3/SDL_pixels.h"

#define RENDER_WIDTH 640
#define RENDER_HEIGHT 480

#define FONT_SIZE 13

#define ANCHOR_TOP_LEFT 1
#define ANCHOR_TOP_MIDDLE 2
#define ANCHOR_TOP_RIGHT 3
#define ANCHOR_MIDDLE_LEFT 4
#define ANCHOR_MIDDLE_MIDDLE 5
#define ANCHOR_MIDDLE_RIGHT 6
#define ANCHOR_BOTTOM_LEFT 7
#define ANCHOR_BOTTOM_MIDDLE 8
#define ANCHOR_BOTTOM_RIGHT 9

constexpr SDL_Color COLOR_WHITE = {255, 255, 255, SDL_ALPHA_OPAQUE};
constexpr SDL_Color COLOR_BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
