#include "aabb.hpp"

AABB::AABB( const Vector2 &position, const Vector2 &half_size )
    : position(position),
      half_size(half_size) {}
