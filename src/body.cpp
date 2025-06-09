#include "body.hpp"

Body::Body( const AABB &aabb, const Vector2 &acceleration, const Vector2 &velocity )
    : aabb(aabb),
      velocity(velocity),
      acceleration(acceleration) {}

Body::Body( const Vector2 &pos, const Vector2 &half_size, const bool filled )
    : aabb(pos, half_size, true, filled) {}


StaticBody::StaticBody( const AABB &aabb )
    : aabb(aabb) {}

StaticBody::StaticBody( const Vector2 &pos, const Vector2 &half_size, const bool filled )
    : aabb(pos, half_size, true, filled) {}
