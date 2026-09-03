package ui

import "core:strings"
import "engine:app"
import "engine:render"
import "engine:util/timer"
import SDL "vendor:sdl3"
import TTF "vendor:sdl3/ttf"

// colors
WHITE :: [4]u8{255, 255, 255, 255}
GRAY :: [4]u8{128, 128, 128, 255}
BLACK :: [4]u8{0, 0, 0, 255}
RED :: [4]u8{255, 0, 0, 255}
BLUE :: [4]u8{0, 0, 255, 255}
GREEN :: [4]u8{0, 255, 0, 255}

ctx: UIContext

UIContext :: struct {
	elements:               [dynamic]Element,
	pointer:                PointerState,
	renderCommandQueue:     [dynamic]RenderCommand,
	hoveredByIndex:         [dynamic]bool,
	previousHoveredByIndex: [dynamic]bool,
	elementIndex:           int,
}

RenderCommand :: struct {
	type:    CommandType,
	element: Element,
	data:    CommandData,
}

CommandType :: enum {
	Rectangle,
	Text,
}

CommandData :: union {
	TextData,
}

Layout :: struct {
	// sizing of the element
	sizing: Sizing,
	// anchor from the parent element (nil/empty for a floating element)
	anchor: bit_set[Anchor],
	// origin of the element
	origin: bit_set[Anchor],
}

Style :: struct {
	// RGBA
	color:  [4]u8,
	// not applicable to text
	filled: bool,
}

Element :: struct {
	// {x, y, width, height}
	rect:   [4]f32,
	layout: Layout,
	style:  Style,
}

TextElement :: struct {
	pos:    [2]f32,
	layout: Layout,
	style:  Style,
}

TextData :: struct {
	text: ^TTF.Text,
}

PointerState :: struct {
	position:   [2]f32,
	button:     SDL.MouseButtonFlags,
	clickTimer: timer.TimerMS,
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

Sizing :: struct {
	width:  SizeMode,
	height: SizeMode,
}

SizeMode :: enum {
	Fixed,
	Grow,
}

@(private)
axis_offset :: proc(anchor: bit_set[Anchor], size: [2]f32) -> [2]f32 {
	offset := [2]f32{}

	if .Right in anchor {
		offset.x = size.x
	} else if .Center in anchor {
		offset.x = size.x / 2
	}

	if .Top in anchor {
		offset.y = 0
	} else if .Bottom in anchor {
		offset.y = size.y
	} else if .Center in anchor {
		offset.y = size.y / 2
	}

	return offset
}

@(private)
calculate_position :: proc(e: ^Element, container: [4]f32) {
	e.rect.xy += container.xy
	e.rect.xy += axis_offset(e.layout.anchor, container.zw)
	e.rect.xy -= axis_offset(e.layout.origin, e.rect.zw)
}

@(private)
calculate_sizing :: proc(e: ^Element, container: [4]f32) {
	if e.layout.sizing.width == .Grow {
		e.rect.z = container.z
	}
	if e.layout.sizing.height == .Grow {
		e.rect.w = container.w
	}
}

begin_frame :: proc() {
	clear(&ctx.elements)
	clear(&ctx.renderCommandQueue)

	clear(&ctx.previousHoveredByIndex)
	reserve(&ctx.previousHoveredByIndex, len(ctx.hoveredByIndex))
	for hovered in ctx.hoveredByIndex {
		append(&ctx.previousHoveredByIndex, hovered)
	}
	clear(&ctx.hoveredByIndex)
	ctx.elementIndex = 0
}

@(private)
open_element :: proc(e: Element) -> bool {
	_open_element(e)
	return true
}

@(private)
_open_element :: proc(e: Element) {
	if len(ctx.elements) == 0 && len(ctx.renderCommandQueue) == 0 {
		ctx.elementIndex = 0
	}

	new := e
	if len(ctx.elements) > 0 {
		parentElement := ctx.elements[len(ctx.elements) - 1]
		calculate_sizing(&new, parentElement.rect)
		calculate_position(&new, parentElement.rect)
	} else {
		screen := [4]f32{0, 0, f32(app.windowSize.x), f32(app.windowSize.y)}
		calculate_sizing(&new, screen)
		calculate_position(&new, screen)
	}

	isHovered :=
		((ctx.pointer.position.x >= new.rect.x) &&
			(ctx.pointer.position.x < (new.rect.x + new.rect.z)) &&
			(ctx.pointer.position.y >= new.rect.y) &&
			(ctx.pointer.position.y < (new.rect.y + new.rect.w)))
	if ctx.elementIndex < len(ctx.hoveredByIndex) {
		ctx.hoveredByIndex[ctx.elementIndex] = isHovered
	} else {
		append(&ctx.hoveredByIndex, isHovered)
	}

	append(&ctx.elements, new)


	append(&ctx.renderCommandQueue, RenderCommand{type = .Rectangle, element = new})

	ctx.elementIndex += 1
}

@(private)
close_element :: proc() {
	pop_safe(&ctx.elements)
}

@(deferred_none = close_element)
rectangle :: proc() -> proc(e: Element) -> bool {
	return open_element
}

text :: proc(str: string, t: TextElement) {
	text := TTF.CreateText(app.textEngine, app.font, strings.clone_to_cstring(str), len(str))

	e: Element
	e.layout = t.layout
	e.style = t.style
	e.rect.xy = t.pos

	tw, th: i32
	TTF.GetTextSize(text, &tw, &th)
	e.rect.zw = {f32(tw), f32(th)}

	if len(ctx.elements) > 0 {
		parentElement := ctx.elements[len(ctx.elements) - 1]
		calculate_sizing(&e, parentElement.rect)
		calculate_position(&e, parentElement.rect)
	} else {
		screen := [4]f32{0, 0, f32(app.windowSize.x), f32(app.windowSize.y)}
		calculate_sizing(&e, screen)
		calculate_position(&e, screen)
	}

	append(
		&ctx.renderCommandQueue,
		RenderCommand{type = .Text, data = TextData{text}, element = e},
	)
}

update_pointer_state :: proc(pos: [2]f32, button: SDL.MouseButtonFlags) {
	ctx.pointer.position = pos
	ctx.pointer.button = button
}

current_element_index :: proc() -> int {
	if len(ctx.hoveredByIndex) == 0 do return 0

	index := ctx.elementIndex
	if index >= len(ctx.hoveredByIndex) do index = len(ctx.hoveredByIndex) - 1
	if index < 0 do return 0
	return index
}

previous_hovered :: proc() -> bool {
	index := current_element_index()
	if index >= len(ctx.previousHoveredByIndex) do return false
	return ctx.previousHoveredByIndex[index]
}

hovered :: proc() -> bool {
	index := current_element_index()
	if index >= len(ctx.hoveredByIndex) do return false
	return ctx.hoveredByIndex[index]
}

entered :: proc() -> bool {
	return hovered() && !previous_hovered()
}

pressed :: proc() -> bool {
	return hovered() && .LEFT in ctx.pointer.button
}

// TODO: implement click detection (basically just a timer)
clicked :: proc() -> bool {
	return pressed()
}

draw :: proc(renderer: ^SDL.Renderer) {
	defer clear(&ctx.renderCommandQueue)
	for cmd in ctx.renderCommandQueue {
		switch cmd.type {
		case .Rectangle:
			render.draw_rect_screen(
				renderer,
				cmd.element.rect,
				cmd.element.style.filled,
				cmd.element.style.color,
			)
		case .Text:
			data := cmd.data.(TextData)
			TTF.SetTextColor(
				data.text,
				cmd.element.style.color.r,
				cmd.element.style.color.g,
				cmd.element.style.color.b,
				cmd.element.style.color.a,
			)
			TTF.DrawRendererText(data.text, cmd.element.rect.x, cmd.element.rect.y)
		}
	}
}

