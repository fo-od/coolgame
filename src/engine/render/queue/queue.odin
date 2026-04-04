package queue

import "core:fmt"
import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

@(private)
renderQ: [dynamic]Command

@(private)
CommandType :: enum u8 {
	RECT,
	FILLED_RECT,
	TEXT,
	DEBUG_TEXT,
}

@(private)
Command :: struct {
	type:     CommandType,
	color:    SDL.Color,
	rectOrXY: union {
		^SDL.FRect,
		[2]f32,
	},
	text:     union {
		cstring,
		^TTF.Text,
	},
}

drawFilledRect :: proc(rect: ^SDL.FRect, color := SDL.Color{255, 255, 255, 255}) {
	append(&renderQ, Command{.FILLED_RECT, color, rect, nil})
}

drawRect :: proc(rect: ^SDL.FRect, color := SDL.Color{255, 255, 255, 255}) {
	append(&renderQ, Command{.RECT, color, rect, nil})
}

drawText :: proc(x, y: f32, text: ^TTF.Text, color := SDL.Color{255, 255, 255, 255}) {
	TTF.SetTextColor(text, color.r, color.g, color.b, color.a)
	append(&renderQ, Command{.TEXT, color, [2]f32{x, y}, text})
}

drawDebugText :: proc(x, y: f32, text: cstring, color := SDL.Color{255, 255, 255, 255}) {
	append(&renderQ, Command{.DEBUG_TEXT, color, [2]f32{x, y}, text})
}

drawDebugTextFormat :: proc(
	x, y: f32,
	format: string,
	args: ..any,
	color := SDL.Color{255, 255, 255, 255},
) {
	drawDebugText(x, y, fmt.ctprintf(format, args), color)
}

render :: proc(renderer: ^SDL.Renderer) {
	for cmd in renderQ {
		if cmd.type != .TEXT do SDL.SetRenderDrawColor(renderer, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a)
		
		switch cmd.type {
		case .RECT:
			SDL.RenderRect(renderer, cmd.rectOrXY.(^SDL.FRect))
		case .FILLED_RECT:
			SDL.RenderFillRect(renderer, cmd.rectOrXY.(^SDL.FRect))
		case .TEXT:
			TTF.DrawRendererText(
				cmd.text.(^TTF.Text),
				cmd.rectOrXY.([2]f32).x,
				cmd.rectOrXY.([2]f32).y,
			)
		case .DEBUG_TEXT:
			SDL.RenderDebugText(
				renderer,
				cmd.rectOrXY.([2]f32).x,
				cmd.rectOrXY.([2]f32).y,
				cmd.text.(cstring),
			)
		}
	}
	clear(&renderQ)
}

