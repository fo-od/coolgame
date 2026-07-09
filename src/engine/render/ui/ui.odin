package ui

Element :: struct {
	// position based off anchor
	offset:       [2]f32,
	// rect with position based off top left corner {x, y, width, height}
	rect:         [4]f32,
	// anchor on the screen (nil for a floating element)
	screenAnchor: bit_set[Anchor],
	// origin of the element
	origin:       bit_set[Anchor],
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

// TODO: refactor screen anchors its weird rn
calculate_position :: proc(e: ^Element, canvas: [2]f32) {
	// Anchor handling
	if (e.screenAnchor != nil) {
		if (e.screenAnchor == {.Center}) {
			e.rect.xy = (canvas.xy / 2) + e.offset.xy
		} else {
			if .Center in e.screenAnchor {
				if .Top in e.screenAnchor || .Bottom in e.screenAnchor {
					e.rect.x = (canvas.x / 2) + e.offset.x
				} else {
					e.rect.y = (canvas.y / 2) - e.offset.y
				}
			}
			if .Bottom in e.screenAnchor {
				e.rect.y = canvas.y - e.offset.y
			}
			if .Right in e.screenAnchor {
				e.rect.x = canvas.x + e.offset.x
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

