package app

import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

// build constants
DEBUG :: #config(GAME_DEBUG, false)
GIT_HASH :: #config(GIT_HASH, "invalid hash")

// global variables
window: ^SDL.Window
windowSize: [2]i32 = {640, 480}
renderer: ^SDL.Renderer
font: ^TTF.Font
textEngine: ^TTF.TextEngine

keyboardState: [^]bool
mouse: Mouse

Mouse :: struct {
	pos:    [2]f32,
	button: SDL.MouseButtonFlags,
}

