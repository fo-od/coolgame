package queue

import "core:fmt"
import "engine:render"
import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

@(private)
commandQueue: [dynamic]Command
@(private)
queueRects: [dynamic]SDL.FRect

@(private)
CommandType :: enum u8 {
	RECT,
	FILLED_RECT,
	TEXT,
	DEBUG_TEXT,
}

@(private)
Command :: struct {
	type:      CommandType,
	color:     SDL.Color,
	rectOrPos: union {
		^SDL.FRect,
		[2]f32,
	},
	text:      union {
		cstring,
		^TTF.Text,
	},
}

drawRect :: proc {
	_drawRect_sdl,
	_drawRect_vec4,
}

@(private)
_drawRect_sdl :: proc(rect: ^SDL.FRect, color := SDL.Color{255, 255, 255, 255}) {
	cmd := Command {
		type      = .RECT,
		color     = color,
		rectOrPos = rect,
	}
	append(&commandQueue, cmd)
}

@(private)
_drawRect_vec4 :: proc(rect: [4]f32, color := SDL.Color{255, 255, 255, 255}) {
	append(&queueRects, SDL.FRect{rect.x, rect.y, rect.z, rect.w})
	cmd := Command {
		type      = .RECT,
		color     = color,
		rectOrPos = &queueRects[len(queueRects) - 1],
	}
	append(&commandQueue, cmd)
}

drawFilledRect :: proc {
	_drawFilledRect_sdl,
	_drawFilledRect_vec4,
}

@(private)
_drawFilledRect_sdl :: proc(rect: ^SDL.FRect, color := SDL.Color{255, 255, 255, 255}) {
	cmd := Command {
		type      = .FILLED_RECT,
		color     = color,
		rectOrPos = rect,
	}
	append(&commandQueue, cmd)
}

@(private)
_drawFilledRect_vec4 :: proc(rect: [4]f32, color := SDL.Color{255, 255, 255, 255}) {
	append(&queueRects, SDL.FRect{rect.x, rect.y, rect.z, rect.w})
	cmd := Command {
		type      = .FILLED_RECT,
		color     = color,
		rectOrPos = &queueRects[len(queueRects) - 1],
	}
	append(&commandQueue, cmd)
}

drawText :: proc(pos: [2]f32, text: ^TTF.Text, color := SDL.Color{255, 255, 255, 255}) {
	cmd := Command {
		type      = .TEXT,
		color     = color,
		rectOrPos = pos,
		text      = text,
	}
	append(&commandQueue, cmd)
}

drawDebugText :: proc(pos: [2]f32, text: cstring, color := SDL.Color{255, 255, 255, 255}) {
	cmd := Command {
		type      = .DEBUG_TEXT,
		color     = color,
		rectOrPos = pos,
		text      = text,
	}
	append(&commandQueue, cmd)
}

drawDebugTextFormat :: proc(
	pos: [2]f32,
	format: string,
	args: ..any,
	color := SDL.Color{255, 255, 255, 255},
) {
	drawDebugText(pos, fmt.ctprintf(format, args), color)
}

render :: proc(renderer: ^SDL.Renderer) {
	for cmd in commandQueue {
		if cmd.type != .TEXT do render.setDrawColor(renderer, cmd.color)

		switch cmd.type {
		case .RECT:
			SDL.RenderRect(renderer, cmd.rectOrPos.(^SDL.FRect))
		case .FILLED_RECT:
			SDL.RenderFillRect(renderer, cmd.rectOrPos.(^SDL.FRect))
		case .TEXT:
			TTF.SetTextColor(
				cmd.text.(^TTF.Text),
				cmd.color.r,
				cmd.color.g,
				cmd.color.b,
				cmd.color.a,
			)
			TTF.DrawRendererText(
				cmd.text.(^TTF.Text),
				cmd.rectOrPos.([2]f32).x,
				cmd.rectOrPos.([2]f32).y,
			)
		case .DEBUG_TEXT:
			SDL.RenderDebugText(
				renderer,
				cmd.rectOrPos.([2]f32).x,
				cmd.rectOrPos.([2]f32).y,
				cmd.text.(cstring),
			)
		}
	}
	clear(&commandQueue)
	clear(&queueRects)
}

