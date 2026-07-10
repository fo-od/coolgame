package ui

import "core:fmt"
import "engine:app"
import "engine:render"

state: UIState

UIState :: struct {
	currentElement:     Maybe(Element),
	pointer:            PointerState,
	renderCommandQueue: [dynamic]RenderCommand,
}

PointerState :: struct {
	position: [2]f32,
	pressed:  bool,
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
	color:  Color,
	filled: bool,
	anchor: bit_set[Anchor],
	origin: bit_set[Anchor],
}

TextData :: struct {
	text:   string,
	config: TextConfig,
}

TextConfig :: struct {
	fontSize:  u16,
	textColor: Color,
	anchor:    Anchor,
}

Element :: struct {
	// {x, y, width, height}
	rect:         [4]f32,
	// anchor on the screen (nil for a floating element)
	screenAnchor: bit_set[Anchor],
	// origin of the element
	origin:       bit_set[Anchor],
}

Color :: [4]u8

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

// TODO: refactor
calculate_position :: proc(e: ^Element, container: [2]f32) {
	// Anchor handling
	if (e.screenAnchor != nil) {
		if (e.screenAnchor == {.Center}) {
			e.rect.xy += (container.xy / 2)
		} else {
			if .Center in e.screenAnchor {
				if .Top in e.screenAnchor || .Bottom in e.screenAnchor {
					e.rect.x += (container.x / 2)
				} else {
					e.rect.y -= (container.y / 2)
				}
			}
			if .Bottom in e.screenAnchor {
				e.rect.y -= container.y
			}
			if .Right in e.screenAnchor {
				e.rect.x += container.x
			}
		}
	}

	// Origin handling
	if (e.origin == {.Center}) {
		e.rect.xy -= e.rect.zw / 2
	} else {
		if .Center in e.origin {
			if .Top in e.origin || .Bottom in e.origin {
				e.rect.x -= e.rect.z / 2
			} else {
				e.rect.y -= e.rect.w / 2
			}
		}
		if .Bottom in e.origin {
			e.rect.y -= e.rect.w
		}
		if .Right in e.origin {
			e.rect.x += e.rect.z
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
	state.currentElement = e
	append(
		&state.renderCommandQueue,
		RenderCommand {
			type = .Rectangle,
			data = RectangleData {
				e.rect,
				{
					color = {255, 0, 0, 255},
					filled = true,
					anchor = e.screenAnchor,
					origin = e.origin,
				},
			},
		},
	)
}

@(private, deferred_none = close_text_element)
open_text_element :: proc(str: string, c: TextConfig) {
}

@(private)
close_element :: proc() {
	state.currentElement = nil
}

@(private)
close_text_element :: proc() {
}

update_pointer_state :: proc(pos: [2]f32, pressed: bool) {
	state.pointer = {pos, pressed}
}

hovered :: proc() -> bool {
	return(
		(state.pointer.position.x >= state.currentElement.(Element).rect.x) &&
		(state.pointer.position.x <
				(state.currentElement.(Element).rect.x + state.currentElement.(Element).rect.z)) &&
		(state.pointer.position.y >= state.currentElement.(Element).rect.y) &&
		(state.pointer.position.y <
				(state.currentElement.(Element).rect.y + state.currentElement.(Element).rect.w)) \
	)
}

text :: proc(str: string, c: TextConfig) {
	// TODO: implement
	open_text_element(str, c)
	fmt.println(str)
}


draw :: proc(renderer: ^SDL.Renderer) {
	defer clear(&state.renderCommandQueue)
	for cmd in state.renderCommandQueue {
		switch cmd.type {
		case .Rectangle:
			data := cmd.data.(RectangleData)
			render.draw_rect_screen(renderer, data.rect, data.config.filled, data.config.color)
		case .Text:
			data := cmd.data.(TextData)
			fmt.printfln(data.text)
		}
	}
}

