#include "engine/body.hpp"

Body::Body( const AABB& aabb, const Vector2& acceleration, const Vector2& velocity ) : mAABB(aabb),
    mVelocity(velocity),
    mAcceleration(acceleration) {}

Body::Body( const Vector2& pos, const Vector2& half_size, const bool filled ) : mAABB(pos, half_size, true, filled) {}


StaticBody::StaticBody( const AABB& aabb ) : mAABB(aabb) {}

StaticBody::StaticBody( const Vector2& pos, const Vector2& half_size, const bool filled ) : mAABB(pos, half_size, true,
         filled) {}