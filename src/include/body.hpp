#pragma once

#include "aabb.hpp"

class Body
{
    AABB aabb;
    Vector2 velocity;
    Vector2 acceleration;

    public:
        Body() = default;
};
