#pragma once

#include "aabb.hpp"

class Body
{
    public:
        AABB aabb{};
        Vector2 velocity{};
        Vector2 acceleration{};

        Body() = default;

        explicit Body( const AABB &aabb, const Vector2 &acceleration = Vector2{}, const Vector2 &velocity = Vector2{} );

        Body( const Vector2 &pos, const Vector2 &half_size, bool filled = false );

        Body( const Body &other ) = default;

        ~Body() = default;
};


class StaticBody
{
    public:
        AABB aabb{};

        StaticBody() = default;

        explicit StaticBody( const AABB &aabb );

        StaticBody( const Vector2 &pos, const Vector2 &half_size, bool filled = false );

        StaticBody( const StaticBody &other ) = default;

        ~StaticBody() = default;
};
