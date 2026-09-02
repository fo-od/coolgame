package app

import SDL "vendor:sdl3"
import "vendor:sdl3/mixer"
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
audioMixer: ^mixer.Mixer

cameraPos: [2]f32

keyboardState: [^]bool
mouse: Mouse

Mouse :: struct {
	pos:    [2]f32,
	button: SDL.MouseButtonFlags,
}

