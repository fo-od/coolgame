package render

import SDL "vendor:sdl3"

Rect :: struct {
	rect:            SDL.FRect,
	color:           SDL.Color,
	visible, filled: bool,
}

@(optimization_mode = "none") // https://github.com/odin-lang/Odin/issues/6809
draw_rect_screen :: proc(
	renderer: ^SDL.Renderer,
	rect: [4]f32,
	filled := false,
	color := [4]u8{255, 255, 255, 255},
) {
	SDL.SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)

	rect := SDL.FRect{rect.x, rect.y, rect.z, rect.w}
	if filled {
		SDL.RenderFillRect(renderer, &rect)
	} else {
		SDL.RenderRect(renderer, &rect)
	}
}

