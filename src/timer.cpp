#include "timer.hpp"

#include "SDL3/SDL_timer.h"

void Timer::start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void Timer::stop()
{
    started = false;
    paused = false;
    startTicks = 0;
    pausedTicks = 0;
}

void Timer::pause()
{
    if ( started && !paused ) {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::resume()
{
    if ( paused && !started ) {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

u64 Timer::get_ticks() const
{
    if ( started ) {
        if ( paused ) {
            return pausedTicks;
        }
        return SDL_GetTicks() - startTicks;
    }
    return 0;
}

bool Timer::is_started() const { return started; }
bool Timer::is_paused() const { return paused; }
