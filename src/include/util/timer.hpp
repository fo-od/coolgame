#pragma once

#include "util/types.hpp"
#include <SDL3/SDL_timer.h>

class Timer
{
    u64 mStartTicks{};
    u64 mPausedTicks{};

    bool mStarted = false;
    bool mPaused = false;

    public:
        Timer() = default;

        ~Timer() = default;

        void start()
        {
            mStarted = true;
            mPaused = false;
            mStartTicks = SDL_GetTicks();
            mPausedTicks = 0;
        }

        void stop()
        {
            mStarted = false;
            mPaused = false;
            mStartTicks = 0;
            mPausedTicks = 0;
        }

        void pause()
        {
            if ( mStarted && !mPaused )
            {
                mPaused = true;
                mPausedTicks = SDL_GetTicks() - mStartTicks;
                mStartTicks = 0;
            }
        }

        void resume()
        {
            if ( mPaused && !mStarted )
            {
                mPaused = false;
                mStartTicks = SDL_GetTicks() - mPausedTicks;
                mPausedTicks = 0;
            }
        }

        /**
         * @returns time since start in milliseconds,
         * @returns the time when it was paused if paused,
         * @returns or 0 if not started
         */
        [[nodiscard]] u64 get_ticks() const
        {
            if ( mStarted )
            {
                if ( mPaused )
                {
                    return mPausedTicks;
                }
                return SDL_GetTicks() - mStartTicks;
            }
            return 0;
        }

        [[nodiscard]] bool is_started() const { return mStarted; }

        [[nodiscard]] bool is_paused() const { return mPaused; }
};