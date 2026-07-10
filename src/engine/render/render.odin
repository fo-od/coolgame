package render

import "core:fmt"
import SDL "vendor:sdl3"

Rect :: struct {
	rect:            SDL.FRect,
	color:           SDL.Color,
	visible, filled: bool,
}

draw_rect_screen :: proc(
	renderer: ^SDL.Renderer,
	rect: [4]f32,
	filled := false,
	color := [4]u8{255, 255, 255, 255},
) {
	// FIXME: for some reason the color is always white if this print statement isnt here?
	fmt.println(color)

	SDL.SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)
	rect := SDL.FRect{rect.x, rect.y, rect.z, rect.w}
	if filled {
		SDL.RenderFillRect(renderer, &rect)
	} else {
		SDL.RenderRect(renderer, &rect)
	}
}

