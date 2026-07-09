package render

import SDL "vendor:sdl3"

Rect :: struct {
	rect:            SDL.FRect,
	color:           SDL.Color,
	visible, filled: bool,
}

// little helper so i can use a struct for color
setDrawColor :: proc(renderer: ^SDL.Renderer, color: SDL.Color) {
	SDL.SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)
}

