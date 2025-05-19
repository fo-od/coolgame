#pragma once

#include "SDL3_ttf/SDL_ttf.h"

inline SDL_Window *window = nullptr;
inline SDL_Renderer *renderer = nullptr;
inline TTF_TextEngine *textEngine = nullptr;
inline TTF_Font *font = nullptr;
inline float deltaTime;
inline bool gameIsRunning = false;
inline bool inMainMenu = true;
