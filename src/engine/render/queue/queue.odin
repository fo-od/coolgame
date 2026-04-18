package queue

import "core:fmt"
import "engine:app"
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

// draw a rectangle in screen space
drawRect_screen :: proc {
	_drawRect_screen_sdl,
	_drawRect_screen_vec4,
}

@(private)
_drawRect_screen_sdl :: proc(rect: ^SDL.FRect, filled := false, color := SDL.Color{255, 255, 255, 255}) {
	cmd := Command {
		type      = .FILLED_RECT if filled else .RECT,
		color     = color,
		rectOrPos = rect,
	}
	append(&commandQueue, cmd)
}

@(private)
_drawRect_screen_vec4 :: proc(rect: [4]f32, filled := false, color := SDL.Color{255, 255, 255, 255}) {
	append(&queueRects, SDL.FRect{rect.x, rect.y, rect.z, rect.w})
	cmd := Command {
		type      = .FILLED_RECT if filled else .RECT,
		color     = color,
		rectOrPos = &queueRects[len(queueRects) - 1],
	}
	append(&commandQueue, cmd)
}

// draw a rectangle in world space
drawRect_world :: proc {
	_drawRect_world_sdl,
	_drawRect_world_vec4,
}

@(private)
_drawRect_world_sdl :: proc(rect: ^SDL.FRect, filled := false, color := SDL.Color{255, 255, 255, 255}) {
	newRect := rect
	newRect.x += app.cameraPos.x
	newRect.y += app.cameraPos.y
	drawRect(newRect, filled, color)
}

@(private)
_drawRect_world_vec4 :: proc(rect: [4]f32, filled := false, color := SDL.Color{255, 255, 255, 255}) {
	newRect := rect
	newRect.xy += app.cameraPos
	drawRect(newRect, filled, color)
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

