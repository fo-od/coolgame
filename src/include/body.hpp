#pragma once

#include "aabb.hpp"

class Body
{
    public:
        AABB aabb{};
        Vector2 velocity{};
        Vector2 acceleration{};

        Body() = default;

        Body( const AABB &aabb, const Vector2 &velocity, const Vector2 &acceleration );

        explicit Body( const AABB &aabb );

        explicit Body( const Body *other );
};
