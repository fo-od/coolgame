#pragma once

#include "aabb.hpp"

class Body
{
    public:
        AABB aabb;
        Vector2 velocity;
        Vector2 acceleration;

        Body() = default;

        Body( AABB aabb, Vector2 velocity, Vector2 acceleration );

        explicit Body( Body *other );
};
