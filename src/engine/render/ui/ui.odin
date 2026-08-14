package ui

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
	hoveredByIndex:     [dynamic]bool,
	elementIndex:       int,
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
	position: [2]f32,
	button:   SDL.MouseButtonFlags,
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

calculate_position :: proc(e: ^Element, container: [4]f32) {
	e.rect.xy += container.xy
	e.rect.xy += axis_offset(e.layout.anchor, container.zw)
	e.rect.xy -= axis_offset(e.layout.origin, e.rect.zw)
}

calculate_sizing :: proc(e: ^Element, container: [4]f32) {
	if e.layout.sizing.width == .Grow {
		e.rect.z = container.z
	}
	if e.layout.sizing.height == .Grow {
		e.rect.w = container.w
	}
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
	TTF.SetTextColor(text, t.style.color.r, t.style.color.g, t.style.color.b, t.style.color.a)

	e: Element
	e.layout = t.layout
	e.style = t.style
	e.rect.xy = t.pos

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

	append(
		&ctx.renderCommandQueue,
		RenderCommand{type = .Text, data = TextData{text}, element = e},
	)
}

update_pointer_state :: proc(pos: [2]f32, button: SDL.MouseButtonFlags) {
	ctx.pointer = {pos, button}
}

hovered :: proc() -> bool {
	if ctx.elementIndex >= len(ctx.hoveredByIndex) do return false
	return ctx.hoveredByIndex[ctx.elementIndex]
}

pressed :: proc() -> bool {
	return hovered() && .LEFT in ctx.pointer.button
}

draw :: proc(renderer: ^SDL.Renderer) {
	defer clear(&ctx.renderCommandQueue)
	for cmd in ctx.renderCommandQueue {
		switch cmd.type {
		case .Rectangle:
			render.draw_rect_screen(renderer, cmd.element.rect, cmd.element.style.filled, cmd.element.style.color)
		case .Text:
			data := cmd.data.(TextData)
			TTF.DrawRendererText(data.text, cmd.element.rect.x, cmd.element.rect.y)
		}
	}
}

