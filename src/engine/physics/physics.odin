package physics

import "core:math"

Body :: struct {
	aabb:                   AABB,
	velocity, acceleration: [2]f32,
}

StaticBody :: struct {
	aabb: AABB,
}

bodies: [dynamic]Body
staticBodies: [dynamic]StaticBody
bodyCount: u32 = 0

iterations: u32 = 2
tickRate: f32 = 1.0 / f32(iterations)
gravity: f32 = 100
terminalVelocity: f32 = 10000

sweep_static_bodies :: proc(aabb: ^AABB, velocity: [2]f32) -> Hit {
	result: Hit
	result.time = math.F32_MAX

	for staticBody in staticBodies {
		sum_aabb := staticBody.aabb
		sum_aabb.halfSize += aabb.halfSize

		hit := intersects_pm(&sum_aabb, aabb.pos, velocity)
		if !hit.isHit {
			continue
		}

		if hit.time > result.time {
			result = hit
		} else if hit.time == result.time {
			if abs(velocity.x) > abs(velocity.y) && hit.normal.x != 0 {
				result = hit
			} else if abs(velocity.y) > abs(velocity.x) && hit.normal.y != 0 {
				result = hit
			}
		}
	}

	return result
}

sweep_response :: proc(body: ^Body, velocity: [2]f32) {
	if hit := sweep_static_bodies(&body.aabb, velocity); hit.isHit {
		body.aabb.pos = hit.position

		if hit.normal.x != 0 {
			body.aabb.pos.y += velocity.y
			body.velocity.x = 0
		} else if hit.normal.y != 0 {
			body.aabb.pos.x += velocity.x
			body.velocity.y = 0
		}
	} else {
		body.aabb.pos += velocity
	}
}

stationary_response :: proc(body: ^Body) {
	for staticBody in staticBodies {
		aabb := minkowski_difference(staticBody.aabb, body.aabb)

		min := min(aabb)
		max := max(aabb)

		if min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0 {
			body.aabb.pos += penetration_vector(aabb)
		}
	}
}

tick :: proc(deltaTime: f32) {
	for &body in bodies {
		body.velocity.y += gravity
		if body.velocity.y > terminalVelocity {
			body.velocity.y = terminalVelocity
		}

		body.velocity += body.acceleration

		scaled_velocity := body.velocity * (deltaTime * tickRate)

		for i: u32; i < iterations; i += 1 {
			sweep_response(&body, scaled_velocity)
			stationary_response(&body)
		}
		update_rect(&body.aabb)
	}
}

get_body :: proc(id: int) -> ^Body {
	return &bodies[id]
}

get_static_body :: proc(id: int) -> ^StaticBody {
	return &staticBodies[id]
}

add_body :: proc(
	pos, half_size: [2]f32,
	filled := false,
	velocity: [2]f32 = {0, 0},
	acceleration: [2]f32 = {0, 0},
) -> int {
	return append(
		&bodies,
		Body{create_AABB(pos, half_size, filled = filled), velocity, acceleration},
	)
}

add_static_body :: proc(pos, half_size: [2]f32, filled := false) -> int {
	return append(&staticBodies, StaticBody{create_AABB(pos, half_size, filled = filled)})
}

