package player

import "core:fmt"
import "engine:app"
import "engine:physics"
import SDL "vendor:sdl3"

body: ^physics.Body
isGrounded: bool
joyX: i8

init :: proc(pos: [2]f32, velocity: [2]f32 = {0, 0}) {
	i := physics.add_body(
		pos,
		{10, 10},
		velocity = velocity,
		collisionLayer = .Player,
		on_hit_static = player_on_hit_static,
		filled = true,
	)
	body = physics.get_body(i)
}

player_on_hit_static :: proc(self: ^physics.Body, other: ^physics.StaticBody, hit: physics.Hit) {
	if hit.normal.y > 0 {
		isGrounded = true
	}
}

input :: proc() {
	joyX = -i8(app.keyboardState[SDL.Scancode.LEFT]) + i8(app.keyboardState[SDL.Scancode.RIGHT])

	body.acceleration.x = 100 * f32(joyX)

	if app.keyboardState[SDL.Scancode.UP] {
		if isGrounded {
			isGrounded = false
			body.velocity.y = -2000
		}
	}

	if app.keyboardState[SDL.Scancode.DOWN] {
		body.aabb.pos = {0, 0}
	}
}

