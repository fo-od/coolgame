#include "aabb.hpp"

#include <complex>

#include "SDL3/SDL.h"

AABB::AABB( const Vector2 &position, const Vector2 &half_size )
    : rect(position.x + half_size.x, position.y - half_size.y, half_size.x * 2, half_size.y * 2),
      position(position),
      half_size(half_size) {}


void AABB::update_rect()
{
    rect.x = position.x;
    rect.y = position.y;
}

Vector2 AABB::min() const { return position - half_size; }
Vector2 AABB::max() const { return position + half_size; }

AABB AABB::minkowski_difference( const AABB &other ) const
{
    return AABB{other.position - this->position, other.half_size + this->half_size};
}

bool AABB::intersects( const AABB &other ) const
{
    const AABB diff = minkowski_difference(other);
    const Vector2 min = diff.min(), max = diff.max();

    return ( min.x <= 0
             && max.x >= 0
             && min.y <= 0
             && max.y >= 0 );
}

Vector2 AABB::penetration_vector( const AABB &other ) const
{
    Vector2 result;

    const AABB diff = minkowski_difference(other);
    const Vector2 min = diff.min(), max = diff.max();

    float min_dist = std::abs(min.x);
    result.x = min.x;
    result.y = 0;

    if ( std::abs(max.x) < min_dist ) {
        min_dist = std::abs(max.x);
        result.x = max.x;
    }

    if ( std::abs(min.y) < min_dist ) {
        min_dist = std::abs(min.y);
        result.x = 0;
        result.y = min.y;
    }

    if ( std::abs(max.y) < min_dist ) {
        result.x = 0;
        result.y = max.y;
    }

    return result;
}
