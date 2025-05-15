#pragma once

#include "vector.hpp"
#include "SDL3/SDL_rect.h"

class Player
{
    bool falling = false;
    bool dashing = false;

    public:
        SDL_FRect rect{};
        Vector2 vel{};
        float speed{}, gravity{}, jumpStrength{}, friction{}, drag{};

        Player( float x, float y, float speed, float gravity, float jumpHeight, float friction, float drag,
                const Vector2 &startingVelocity );

        Player( float x, float y, float speed, float gravity, float jumpHeight, float friction,
                float drag );

        ~Player() = default;

        void update();

        void draw() const;
};
