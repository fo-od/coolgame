package umath

import "core:math"
// useful decay range 1 - 25 (slow to fast)
expDecay :: proc(a, b, decay, deltaTime: f32) -> f32 {
	return b + (a - b) * math.exp(-decay * deltaTime)
}

abs :: proc {
	abs_vec2_f32,
}

abs_vec2_f32 :: proc(value: [2]f32) -> [2]f32 {
	return {math.abs(value.x), math.abs(value.y)}
}

