package app

import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

// build constants
DEBUG :: #config(GAME_DEBUG, false)
GIT_HASH :: #config(GIT_HASH, "invalid hash")
GIT_HASH_SHORT :: #config(GIT_HASH_SHORT, "invalid hash")

// global variables
window: ^SDL.Window
windowSize: [2]i32 = {640, 480}
renderer: ^SDL.Renderer
font: ^TTF.Font
textEngine: ^TTF.TextEngine

