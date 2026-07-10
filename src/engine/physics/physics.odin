package physics

import "core:fmt"
import "core:math"
import "engine:physics"
import "engine:render"
import "engine:util/umath"
import SDL "vendor:sdl3"

Body :: struct {
	aabb:                   AABB,
	velocity, acceleration: [2]f32,
	collisionMask:          Collision_Mask,
	collisionLayer:         Collision_Layer,
	on_hit:                 proc(self, other: ^Body, hit: Hit),
	on_hit_static:          proc(self: ^Body, other: ^StaticBody, hit: Hit),
}

StaticBody :: struct {
	aabb:           AABB,
	collisionLayer: Collision_Layer,
}

Hit :: struct {
	isHit:            bool,
	otherID:          int,
	time:             f32,
	position, normal: [2]f32,
}

Collision_Layer :: enum {
	Player  = 1,
	Terrain = 1 << 1,
	Layer_3 = 1 << 2,
	Layer_4 = 1 << 3,
	Layer_5 = 1 << 4,
	Layer_6 = 1 << 5,
	Layer_7 = 1 << 6,
	Layer_8 = 1 << 7,
}

Collision_Mask :: bit_set[Collision_Layer]

@(private)
bodies: [dynamic]Body
@(private)
staticBodies: [dynamic]StaticBody

@(private)
iterations: u32 : 4
@(private)
tickRate: f32 : 1.0 / f32(iterations)
@(private)
gravity: f32 : 10000 * 0.75
@(private)
terminalVelocity: f32 : 10000

update :: proc(deltaTime: f32) {
	if deltaTime == 0 do return
	for &body in bodies {
		body.velocity.y += gravity * deltaTime

		body.velocity += body.acceleration * deltaTime

		body.velocity = umath.clamp(body.velocity, -terminalVelocity, terminalVelocity)

		scaled_velocity := body.velocity * (deltaTime * tickRate)

		for i: u32; i < iterations; i += 1 {
			sweep_response(&body, scaled_velocity)
			stationary_response(&body)
		}

		if body.velocity.y == 0 {
			// friction
			body.velocity.x *= 0.9
		} else {
			// air resistance
			body.velocity.x *= 0.95
		}
		if body.velocity.x < 10 && body.velocity.x > -10 do body.velocity.x = 0
	}
}

@(private)
update_sweep_result :: proc(
	result: ^Hit,
	other_id: int,
	a, b: AABB,
	velocity: [2]f32,
	a_collision_mask: Collision_Mask,
	b_collision_layer: Collision_Layer,
) {
	if b_collision_layer not_in a_collision_mask do return

	sum_aabb := b
	sum_aabb.halfSize += a.halfSize

	if hit := aabb_intersects_ray(&sum_aabb, a.pos, velocity); hit.isHit {
		if hit.time < result.time {
			result^ = hit
		} else if hit.time == result.time {
			if abs(velocity.x) > abs(velocity.y) && hit.normal.x != 0 {
				result^ = hit
			} else if abs(velocity.y) > abs(velocity.x) && hit.normal.y != 0 {
				result^ = hit
			}
		}
		result.otherID = other_id
	}
}

@(private)
sweep_static_bodies :: proc(body: ^Body, velocity: [2]f32) -> Hit {
	result := Hit {
		time = math.F32_MAX,
	}

	for staticBody, i in staticBodies {
		update_sweep_result(
			&result,
			i,
			body.aabb,
			staticBody.aabb,
			velocity,
			body.collisionMask,
			staticBody.collisionLayer,
		)
	}

	return result
}

@(private)
sweep_bodies :: proc(body: ^Body, velocity: [2]f32) -> Hit {
	result := Hit {
		time = math.F32_MAX,
	}

	for &other, i in bodies {
		if &other == body do continue

		update_sweep_result(
			&result,
			i,
			body.aabb,
			other.aabb,
			velocity,
			body.collisionMask,
			other.collisionLayer,
		)
	}

	return result
}

@(private)
sweep_response :: proc(body: ^Body, velocity: [2]f32) {
	// moving response
	if hit := sweep_bodies(body, velocity); hit.isHit {
		if body.on_hit != nil {
			body.on_hit(body, get_body(hit.otherID), hit)
		}
	}

	// static response
	if hit := sweep_static_bodies(body, velocity); hit.isHit {
		body.aabb.pos = hit.position

		if hit.normal.x != 0 {
			body.aabb.pos.y += velocity.y
			body.velocity.x = 0
		} else if hit.normal.y != 0 {
			body.aabb.pos.x += velocity.x
			body.velocity.y = 0
		}

		if body.on_hit_static != nil {
			body.on_hit_static(body, get_static_body(hit.otherID), hit)
		}
	} else {
		body.aabb.pos += velocity
	}
}

@(private)
stationary_response :: proc(body: ^Body) {
	for staticBody in staticBodies {
		if staticBody.collisionLayer not_in body.collisionMask do continue

		aabb := minkowski_difference(staticBody.aabb, body.aabb)

		min, max := aabb_min_max(aabb)

		if min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0 {
			body.aabb.pos += aabb_penetration_vector(aabb)
		}
	}
}

get_body :: proc(id: int) -> ^Body {
	return &bodies[id]
}

get_static_body :: proc(id: int) -> ^StaticBody {
	return &staticBodies[id]
}

add_body :: proc(
	pos, halfSize: [2]f32,
	visible := true,
	filled := false,
	velocity: [2]f32 = {0, 0},
	acceleration: [2]f32 = {0, 0},
	collisionMask := Collision_Mask{.Terrain},
	collisionLayer := Collision_Layer.Terrain,
	on_hit: proc(self, other: ^Body, hit: Hit) = nil,
	on_hit_static: proc(self: ^Body, other: ^StaticBody, hit: Hit) = nil,
) -> int {
	append(
		&bodies,
		Body {
			create_AABB(pos, halfSize, visible, filled),
			velocity,
			acceleration,
			collisionMask,
			collisionLayer,
			on_hit,
			on_hit_static,
		},
	)
	return len(bodies) - 1
}

add_static_body :: proc(
	pos, half_size: [2]f32,
	visible := true,
	filled := false,
	collision_layer := Collision_Layer.Terrain,
) -> int {
	append(
		&staticBodies,
		StaticBody{create_AABB(pos, half_size, visible, filled), collision_layer},
	)
	return len(staticBodies) - 1
}

// TODO: refactor
draw :: proc(renderer: ^SDL.Renderer) {
	for &body in bodies {
		aabb_update_rect(&body.aabb)
		if !body.aabb.rect.visible do continue

		SDL.SetRenderDrawColor(
			renderer,
			body.aabb.rect.color.r,
			body.aabb.rect.color.g,
			body.aabb.rect.color.b,
			body.aabb.rect.color.a,
		)
		if body.aabb.rect.filled {
			SDL.RenderFillRect(renderer, &body.aabb.rect.rect)
		} else {
			SDL.RenderRect(renderer, &body.aabb.rect.rect)
		}
	}
	for &static_body in staticBodies {
		aabb_update_rect(&static_body.aabb)
		if !static_body.aabb.rect.visible do continue

		SDL.SetRenderDrawColor(
			renderer,
			static_body.aabb.rect.color.r,
			static_body.aabb.rect.color.g,
			static_body.aabb.rect.color.b,
			static_body.aabb.rect.color.a,
		)
		if static_body.aabb.rect.filled {
			SDL.RenderFillRect(renderer, &static_body.aabb.rect.rect)
		} else {
			SDL.RenderRect(renderer, &static_body.aabb.rect.rect)
		}
	}
}

