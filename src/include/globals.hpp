#pragma once

#include "util.hpp"

inline SDL_Window *window = nullptr;
inline SDL_Renderer *renderer = nullptr;
inline TTF_TextEngine *textEngine = nullptr;
inline TTF_Font *font = nullptr;
inline Mouse mouse;
inline const bool *keyboardState;
inline float deltaTime;
inline bool gameIsRunning = false;
inline bool inMainMenu = true;
