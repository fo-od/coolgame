package timer

import SDL "vendor:sdl3"

Timer :: struct {
	type:        enum {
		// Millisecond precision
		MS,
		// Nanosecond precision
		NS,
	},
	//The clock time when the timer started
	startTicks:  u64,
	started:     bool,
	//The ticks stored when the timer was paused
	pausedTicks: u64,
	paused:      bool,
}

getTicks :: proc(timer: ^Timer) -> u64 {
	if timer.started {
		if timer.paused {
			return timer.pausedTicks
		}
		return (SDL.GetTicks() if timer.type == .MS else SDL.GetTicksNS()) - timer.startTicks
	}
	return 0
}

start :: proc(timer: ^Timer) {
	timer.started = true
	timer.paused = false

	timer.startTicks = (SDL.GetTicks() if timer.type == .MS else SDL.GetTicksNS())
	timer.pausedTicks = 0
}

stop :: proc(timer: ^Timer) {
	timer.started = false
	timer.paused = false

	timer.startTicks = 0
	timer.pausedTicks = 0
}

pause :: proc(timer: ^Timer) {
	if timer.started && !timer.paused {
		timer.paused = true

		timer.pausedTicks =
			(SDL.GetTicks() if timer.type == .MS else SDL.GetTicksNS()) - timer.startTicks
		timer.startTicks = 0
	}
}

unpause :: proc(timer: ^Timer) {
	if timer.started && timer.paused {
		timer.paused = false

		timer.startTicks =
			(SDL.GetTicks() if timer.type == .MS else SDL.GetTicksNS()) - timer.pausedTicks
		timer.pausedTicks = 0
	}
}

