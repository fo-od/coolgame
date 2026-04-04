package physics

import "core:math"
import SDL "vendor:sdl3"

Hit :: struct {
	isHit:            bool,
	time:             f32,
	position, normal: [2]f32,
}

Rect :: struct {
	rect:            SDL.FRect,
	visible, filled: bool,
}

@(private)
rects: [dynamic]^Rect
filledRects: [dynamic]^Rect

AABB :: struct {
	pos, halfSize: [2]f32,
	rect:          Rect,
}

create_AABB :: proc(position, half_size: [2]f32, visible := false, filled := false) -> AABB {
	rect := Rect {
		{position.x - half_size.x, position.y - half_size.y, half_size.x * 2, half_size.y * 2},
		visible,
		filled,
	}
	aabb := AABB{position, half_size, rect}
	if visible {
		if filled {
			append(&filledRects, &aabb.rect)
		} else {
			append(&rects, &aabb.rect)
		}
	}
	return aabb
}

update_rect :: proc(aabb: ^AABB) {
	aabb.rect.rect.x = aabb.pos.x - aabb.halfSize.x
	aabb.rect.rect.y = aabb.pos.y - aabb.halfSize.y
}

draw :: proc(renderer: ^SDL.Renderer) {
	for rect in rects {
		if !rect.visible do continue

		if rect.filled {
			SDL.RenderRect(renderer, &rect.rect)
		} else {
			SDL.RenderFillRect(renderer, &rect.rect)
		}
	}
}

min :: proc(aabb: AABB) -> [2]f32 {
	return aabb.pos - aabb.halfSize
}

max :: proc(aabb: AABB) -> [2]f32 {
	return aabb.pos + aabb.halfSize
}

minkowski_difference :: proc(a, b: AABB) -> AABB {
	return create_AABB(a.pos - b.pos, a.halfSize + b.halfSize)
}

intersects_aabb :: proc(a, b: AABB) -> bool {
	diff := minkowski_difference(a, b)
	min := min(diff)
	max := max(diff)

	return min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0
}

intersects_pm :: proc(aabb: ^AABB, pos, magnitude: [2]f32) -> Hit {
	hit: Hit
	min := min(aabb^)
	max := max(aabb^)

	last_entry: f32 = math.F32_MIN
	first_exit: f32 = math.F32_MAX

	// repeat for 2 dimensions
	for i := 0; i < 2; i += 1 {
		// avoid divide by 0
		if (magnitude[i] != 0) {
			t1 := (min[i] - pos[i]) / magnitude[i]
			t2 := (max[i] - pos[i]) / magnitude[i]

			last_entry = math.max(last_entry, math.min(t1, t2))
			first_exit = math.min(first_exit, math.max(t1, t2))
		} else if (pos[i] <= min[i] || pos[i] >= max[i]) {
			return hit
		}
	}

	if (first_exit > last_entry && first_exit > 0 && last_entry < 1) {
		hit.position = pos + magnitude * last_entry
		hit.isHit = true
		hit.time = last_entry

		dx: f32 = hit.position.x - aabb.pos.x
		dy: f32 = hit.position.y - aabb.pos.y
		px: f32 = aabb.halfSize.x - abs(dx)
		py: f32 = aabb.halfSize.y - abs(dy)

		if (px < py) {
			hit.normal.x = f32(int(dx > 0) - int(dx < 0))
		} else {
			hit.normal.y = f32(int(dy > 0) - int(dy < 0))
		}
	}

	return hit
}

penetration_vector :: proc(aabb: AABB) -> [2]f32 {
	result: [2]f32

	min := min(aabb)
	max := max(aabb)

	min_dist := abs(min.x)
	result.x = min.x
	result.y = 0

	if abs(max.x) < min_dist {
		min_dist = abs(max.x)
		result.x = max.x
	}

	if abs(min.y) < min_dist {
		min_dist = abs(min.y)
		result.x = 0
		result.y = min.y
	}

	if abs(max.y) < min_dist {
		result.x = 0
		result.y = max.y
	}

	return result
}

