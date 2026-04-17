package physics

import "core:fmt"
import "../render"
import "core:math"
import "engine:app"
import SDL "vendor:sdl3"

AABB :: struct {
	pos, halfSize: [2]f32,
	rect:          render.Rect,
}

create_AABB :: proc(
	position, half_size: [2]f32,
	visible := false,
	filled := false,
	color := SDL.Color{255, 255, 255, 255},
) -> AABB {
	rect := render.Rect {
		{position.x - half_size.x, position.y - half_size.y, half_size.x * 2, half_size.y * 2},
		color,
		visible,
		filled,
	}
	aabb := AABB{position, half_size, rect}
	return aabb
}

update_rect :: proc(aabb: ^AABB) {
	aabb.rect.rect.x = aabb.pos.x - aabb.halfSize.x + app.cameraPos.x
	aabb.rect.rect.y = aabb.pos.y - aabb.halfSize.y + app.cameraPos.y
}

min :: proc(aabb: AABB) -> [2]f32 {
	return aabb.pos - aabb.halfSize
}

max :: proc(aabb: AABB) -> [2]f32 {
	return aabb.pos + aabb.halfSize
}

@(private)
minkowski_difference :: proc(a, b: AABB) -> AABB {
	return create_AABB(a.pos - b.pos, a.halfSize + b.halfSize)
}

@(private)
intersects_aabb :: proc(a, b: AABB) -> bool {
	diff := minkowski_difference(a, b)
	min := min(diff)
	max := max(diff)

	return min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0
}

@(private)
intersects_ray :: proc(aabb: ^AABB, pos, magnitude: [2]f32) -> Hit {
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

		d := hit.position - aabb.pos
		px: f32 = aabb.halfSize.x - abs(d.x)
		py: f32 = aabb.halfSize.y - abs(d.y)

		if (px < py) {
			hit.normal.x = math.sign(d.x)
		} else {
			hit.normal.y = math.sign(d.y)
		}
	}

	return hit
}

@(private)
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

