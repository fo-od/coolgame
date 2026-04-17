package render

import SDL "vendor:sdl3"

Rect :: struct {
	rect:            SDL.FRect,
	color:           SDL.Color,
	visible, filled: bool,
}

@(private)
drawColor := SDL.Color{0, 0, 0, 255}

// only change draw color if it isnt the same as current color
setDrawColor :: proc(renderer: ^SDL.Renderer, color: SDL.Color) {
	if drawColor != color {
		drawColor = color
		SDL.SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)
	}
}

