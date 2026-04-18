package physics

import "core:math"
import "engine:app"
import "engine:render"
import "engine:util/umath"
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

aabb_update_rect :: proc(aabb: ^AABB) {
	aabb.rect.rect.x = aabb.pos.x - aabb.halfSize.x + app.cameraPos.x
	aabb.rect.rect.y = aabb.pos.y - aabb.halfSize.y + app.cameraPos.y
}

aabb_min_max :: proc(aabb: AABB) -> (min: [2]f32, max: [2]f32) {
	return aabb.pos - aabb.halfSize, aabb.pos + aabb.halfSize
}

@(private)
minkowski_difference :: proc(a, b: AABB) -> AABB {
	return create_AABB(a.pos - b.pos, a.halfSize + b.halfSize)
}

@(private)
aabb_intersects_aabb :: proc(a, b: AABB) -> bool {
	diff := minkowski_difference(a, b)
	min, max := aabb_min_max(diff)

	return min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0
}

@(private)
aabb_intersects_ray :: proc(aabb: ^AABB, pos, magnitude: [2]f32) -> Hit {
	hit: Hit
	min, max := aabb_min_max(aabb^)

	last_entry := -math.INF_F32
	first_exit := math.INF_F32

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
		p := aabb.halfSize - umath.abs(d)

		if (p.x < p.y) {
			hit.normal.x = -math.sign(d.x)
		} else {
			hit.normal.y = -math.sign(d.y)
		}
	}

	return hit
}

@(private)
aabb_penetration_vector :: proc(aabb: AABB) -> [2]f32 {
	result: [2]f32

	min, max := aabb_min_max(aabb)

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

