#pragma once

#include "vector.hpp"

class AABB
{
    public:
        Vector2 position;
        Vector2 half_size;

        AABB() = default;

        ~AABB() = default;

        AABB( const Vector2 &position, const Vector2 &half_size );
};
