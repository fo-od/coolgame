package ui

import "core:fmt"
import "core:strings"
import "engine:app"
import "engine:render"
import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

ctx: UIContext

UIContext :: struct {
	elements:           [dynamic]Element,
	pointer:            PointerState,
	renderCommandQueue: [dynamic]RenderCommand,
}

RenderCommand :: struct {
	type: CommandType,
	data: CommandData,
}

CommandType :: enum {
	Rectangle,
	Text,
}

CommandData :: union {
	RectangleData,
	TextData,
}

RectangleData :: struct {
	rect:   [4]f32,
	config: RectangleConfig,
}

RectangleConfig :: struct {
	color:  [4]u8,
	filled: bool,
	anchor: bit_set[Anchor],
	origin: bit_set[Anchor],
}

TextData :: struct {
	text:   ^TTF.Text,
	pos:    [2]f32,
	config: TextConfig,
}

TextConfig :: struct {
	color:  [4]u8,
	anchor: bit_set[Anchor],
}

PointerState :: struct {
	position: [2]f32,
	button:   SDL.MouseButtonFlags,
}

Element :: struct {
	// {x, y, width, height}
	rect:   [4]f32,
	color:  [4]u8,
	// anchor from the parent element (nil for a floating element)
	anchor: bit_set[Anchor],
	// origin of the element
	origin: bit_set[Anchor],
}

// 1----2----3    1=Top+Left     2=Top+Center     3=Top+Right
// |    |    |
// 4----5----6    4=Center+Left  5=Center         6=Center+Right
// |    |    |
// 7----8----9    7=Bottom+Left  8=Bottom+Center  9=Bottom+Right
Anchor :: enum {
	Top,
	Left,
	Center,
	Bottom,
	Right,
}

// TODO: make it shorter?
calculate_position :: proc(e: ^Element, container: [4]f32) {
	// For top and left we do nothing, since thats default.

	// Anchor handling
	if e.anchor != nil || e.anchor != {.Top, .Left} {
		// horizontal
		if .Right in e.anchor {
			e.rect.x += container.z
		}

		// vertical
		if .Top in e.anchor {
			if .Center in e.anchor { 	// 2
				e.rect.x += container.z / 2
			}
		} else if .Center in e.anchor {
			e.rect.y += container.w / 2
			if .Center in e.anchor { 	// 5
				e.rect.x += container.z / 2
			}
		} else if .Bottom in e.anchor {
			e.rect.y += container.w
			if .Center in e.anchor { 	// 8
				e.rect.x += container.z / 2
			}
		}
	}

	// Origin handling
	if e.origin != nil || e.origin != {.Top, .Left} {
		// horizontal
		if .Right in e.origin {
			e.rect.x -= e.rect.z
		}

		// vertical
		if .Top in e.origin {
			if .Center in e.origin { 	// 2
				e.rect.x -= e.rect.z / 2
			}
		} else if .Center in e.origin {
			e.rect.y -= e.rect.w / 2
			if .Center in e.origin { 	// 5
				e.rect.x -= e.rect.z / 2
			}
		} else if .Bottom in e.origin {
			e.rect.y -= e.rect.w
			if .Center in e.origin { 	// 8
				e.rect.x -= e.rect.z / 2
			}
		}
	}
}

@(deferred_none = close_element)
make :: proc() -> proc(e: Element) -> bool {
	return open_element
}

@(private)
open_element :: proc(e: Element) -> bool {
	_open_element(e)
	return true
}

@(private)
_open_element :: proc(e: Element) {
	new := e
	if len(ctx.elements) > 0 {
		parentElement := ctx.elements[len(ctx.elements) - 1]
		calculate_position(&new, parentElement.rect)
	} else {
		calculate_position(&new, {0, 0, f32(app.windowSize.x), f32(app.windowSize.y)})
	}

	append(&ctx.elements, new)

	append(
		&ctx.renderCommandQueue,
		RenderCommand {
			type = .Rectangle,
			data = RectangleData {
				new.rect,
				{color = e.color, filled = true, anchor = e.anchor, origin = e.origin},
			},
		},
	)
}

@(deferred_none = close_element)
text :: proc(str: string, c: TextConfig) {
	text := TTF.CreateText(app.textEngine, app.font, strings.clone_to_cstring(str), len(str))
	TTF.SetTextColor(text, c.color.r, c.color.g, c.color.b, c.color.a)

	e: Element

	tw, th: i32
	TTF.GetTextSize(text, &tw, &th)
	e.rect.zw = {f32(tw), f32(th)}

	// TODO: do cool calculations to get text position :p
	if len(ctx.elements) > 0 {
		parentElement := ctx.elements[len(ctx.elements) - 1]
		e.rect.xy = parentElement.rect.xy
	} else {
		e.rect.xy = {0, 0}
	}

	append(&ctx.elements, e)
	append(
		&ctx.renderCommandQueue,
		RenderCommand{type = .Text, data = TextData{text, e.rect.xy, c}},
	)
}

@(private)
close_element :: proc() {
	pop_safe(&ctx.elements)
}

update_pointer_state :: proc(pos: [2]f32, button: SDL.MouseButtonFlags) {
	ctx.pointer = {pos, button}
}

hovered :: proc() -> bool {
	currentElement := ctx.elements[len(ctx.elements) - 1]
	return(
		(ctx.pointer.position.x >= currentElement.rect.x) &&
		(ctx.pointer.position.x < (currentElement.rect.x + currentElement.rect.z)) &&
		(ctx.pointer.position.y >= currentElement.rect.y) &&
		(ctx.pointer.position.y < (currentElement.rect.y + currentElement.rect.w)) \
	)
}

draw :: proc(renderer: ^SDL.Renderer) {
	defer clear(&ctx.renderCommandQueue)
	for cmd in ctx.renderCommandQueue {
		switch cmd.type {
		case .Rectangle:
			data := cmd.data.(RectangleData)
			render.draw_rect_screen(renderer, data.rect, data.config.filled, data.config.color)
		case .Text:
			data := cmd.data.(TextData)
			TTF.DrawRendererText(data.text, data.pos.x, data.pos.y)
		}
	}
}

