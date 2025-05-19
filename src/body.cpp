#include "body.hpp"

Body::Body( const AABB &aabb, const Vector2 &velocity, const Vector2 &acceleration )
    : aabb(aabb),
      velocity(velocity),
      acceleration(acceleration) {}


Body::Body( const AABB &aabb )
    : aabb(aabb) {}

Body::Body( const Body *other )
    : aabb(other->aabb),
      velocity(other->velocity),
      acceleration(other->acceleration) {}
