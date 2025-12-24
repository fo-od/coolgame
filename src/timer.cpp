#include "timer.hpp"

#include <SDL3/SDL_timer.h>

void Timer::start()
{
    mStarted = true;
    mPaused = false;
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void Timer::stop()
{
    mStarted = false;
    mPaused = false;
    mStartTicks = 0;
    mPausedTicks = 0;
}

void Timer::pause()
{
    if (mStarted && !mPaused) {
        mPaused = true;
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void Timer::resume()
{
    if (mPaused && !mStarted) {
        mPaused = false;
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        mPausedTicks = 0;
    }
}

u64 Timer::get_ticks() const
{
    if (mStarted) {
        if (mPaused) {
            return mPausedTicks;
        }
        return SDL_GetTicks() - mStartTicks;
    }
    return 0;
}

bool Timer::is_started() const { return mStarted; }
bool Timer::is_paused() const { return mPaused; }