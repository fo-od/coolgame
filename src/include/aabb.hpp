#pragma once

#include "vector.hpp"

class AABB
{
    Vector2 position;
    Vector2 half_size;

    public:
        AABB() = default;

        ~AABB() = default;

        AABB( const Vector2 &position, const Vector2 &half_size );
};
