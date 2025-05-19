#pragma once
#include "types.hpp"

class Timer
{
    u64 startTicks{};
    u64 pausedTicks{};

    bool started = false;
    bool paused = false;

    public:
        Timer() = default;

        ~Timer() = default;

        void start();

        void stop();

        void pause();

        void resume();

        /**
         * @returns time since start in milliseconds
         * @returns the time when it was paused if paused
         * @returns 0 if not started
         */
        [[nodiscard]] u64 get_ticks() const;

        [[nodiscard]] bool is_started() const;

        [[nodiscard]] bool is_paused() const;
};
