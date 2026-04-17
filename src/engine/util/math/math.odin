package umath

import "core:math"
// useful decay range 1 - 25 (slow to fast)
expDecay :: proc(a, b, decay, deltaTime: f32) -> f32 {
	return b + (a - b) * math.exp(-decay * deltaTime)
}

