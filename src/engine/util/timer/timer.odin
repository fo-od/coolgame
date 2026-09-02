package timer

import SDL "vendor:sdl3"

@(private)
_Timer :: struct {
	//The clock time when the timer started
	startTicks:  u64,
	started:     bool,
	//The ticks stored when the timer was paused
	pausedTicks: u64,
	paused:      bool,
}

TimerMS :: struct {
	using _timer: _Timer,
}

TimerNS :: struct {
	using _timer: _Timer,
}

getTicks :: proc(timer: ^$T) -> u64 {
	if timer.started {
		if timer.paused {
			return timer.pausedTicks
		}

		return _get_ticks(timer) - timer.startTicks
	}
	return 0
}

start :: proc(timer: ^$T) {
	timer.started = true
	timer.paused = false

	timer.startTicks = _get_ticks(timer)
	timer.pausedTicks = 0
}

stop :: proc(timer: ^$T) {
	timer.started = false
	timer.paused = false

	timer.startTicks = 0
	timer.pausedTicks = 0
}

pause :: proc(timer: ^$T) {
	if timer.started && !timer.paused {
		timer.paused = true

		timer.pausedTicks = _get_ticks(timer) - timer.startTicks
		timer.startTicks = 0
	}
}

unpause :: proc(timer: ^$T) {
	if timer.started && timer.paused {
		timer.paused = false

		timer.startTicks = _get_ticks(timer) - timer.pausedTicks
		timer.pausedTicks = 0
	}
}

@(private)
_get_ticks :: proc(timer: $T) -> u64 {
	if T == TimerMS {
		return SDL.GetTicks()
	} else if T == TimerNS {
		return SDL.GetTicksNS()
	}
	return 0
}

