#pragma once
#include "types.hpp"

class Timer
{
    u64 mStartTicks{};
    u64 mPausedTicks{};

    bool mStarted = false;
    bool mPaused = false;

    public:
        Timer() = default;

        ~Timer() = default;

        void start();

        void stop();

        void pause();

        void resume();

        /**
         * @returns time since start in milliseconds,
         * @returns the time when it was paused if paused,
         * @returns or 0 if not started
         */
        [[nodiscard]] u64 get_ticks() const;

        [[nodiscard]] bool is_started() const;

        [[nodiscard]] bool is_paused() const;
};