package umath

import "core:math"
// useful decay range 1 - 25 (slow to fast)
expDecay :: proc(a, b, decay, deltaTime: f32) -> f32 {
	return b + (a - b) * math.exp(-decay * deltaTime)
}

abs :: proc(value: [2]f32) -> [2]f32 {
	return {math.abs(value.x), math.abs(value.y)}
}

clamp :: proc(value: [2]f32, min, max: f32) -> [2]f32 {
	return {math.clamp(value.x, min, max), math.clamp(value.y, min, max)}
}

