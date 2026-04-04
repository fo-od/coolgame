package timer

import SDL "vendor:sdl3"

TimerNS :: struct {
	//The clock time when the timer started
    startTicks:      u64,
    //The ticks stored when the timer was paused
    pausedTicks:     u64,

    paused, started: bool,
}

getTicksNS :: proc(timer: ^TimerNS) -> u64 {
	if timer.started {
		if timer.paused {
			return timer.pausedTicks
		}
		return SDL.GetTicksNS() - timer.startTicks
	}
	return 0
}

start :: proc(timer: ^TimerNS) {
	timer.started = true
	timer.paused = false
	
	timer.startTicks = SDL.GetTicksNS()
	timer.pausedTicks = 0
}

stop :: proc(timer: ^TimerNS) {
	timer.started = false
	timer.paused = false
	
	timer.startTicks = 0
	timer.pausedTicks = 0
}

pause :: proc(timer: ^TimerNS) {
	if timer.started && !timer.paused {
		timer.paused = true
		
		timer.pausedTicks = SDL.GetTicksNS() - timer.startTicks
		timer.startTicks = 0
	}
}

unpause :: proc(timer: ^TimerNS) {
	if timer.started && timer.paused {
		timer.paused = false
		
		timer.startTicks = SDL.GetTicksNS() - timer.pausedTicks
		timer.pausedTicks = 0
	}
}