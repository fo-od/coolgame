#pragma once
#include "vector.hpp"
#include "SDL3/SDL_rect.h"

class Player
{
    public:
        SDL_FRect rect;
        Vector2 vel;
        float speed, gravity, friction;

        Player() = default;

        Player( float x, float y, float speed, float gravity, float friction, const Vector2 &startingVelocity );

        Player( float x, float y, float speed, float gravity, float friction );

        ~Player() = default;

        void update();

        void draw() const;
};
