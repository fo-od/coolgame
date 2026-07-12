package render

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
	set_draw_color(renderer, color)

	rect := SDL.FRect{rect.x, rect.y, rect.z, rect.w}
	if filled {
		SDL.RenderFillRect(renderer, &rect)
	} else {
		SDL.RenderRect(renderer, &rect)
	}
}

set_draw_color :: proc {
	set_draw_color_v,
	set_draw_color_sdl,
}

@(private, optimization_mode = "none") // https://github.com/odin-lang/Odin/issues/6809
set_draw_color_v :: proc(renderer: ^SDL.Renderer, color: [4]u8) {
	SDL.SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)
}

@(private, optimization_mode = "none") // https://github.com/odin-lang/Odin/issues/6809
set_draw_color_sdl :: proc(renderer: ^SDL.Renderer, color: SDL.Color) {
	SDL.SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)
}

