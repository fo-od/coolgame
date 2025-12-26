#include "physics.hpp"

#include "body.hpp"
#include "types.hpp"
#include <cmath>

std::vector<Body> Physics::bodies;
std::vector<StaticBody> Physics::staticBodies;

u32 Physics::iterations = 2;
float Physics::tickRate = 1.f / iterations;

float Physics::gravity = 100;
float Physics::terminalVelocity = 10000;


Hit Physics::sweep_static_bodies( const AABB &aabb, const Vector2 &velocity )
{
    Hit result{.mTime = 0xBEEF};

    for (const StaticBody &static_body: staticBodies) {
        AABB sum_aabb = static_body.mAABB;
        sum_aabb.mHalfSize += aabb.mHalfSize;

        const Hit hit = sum_aabb.intersects(aabb.mPos, velocity);
        if (!hit.mIsHit) {
            continue;
        }

        if (hit.mTime < result.mTime) {
            result = hit;
        } else if (hit.mTime == result.mTime) {
            if (std::abs(velocity.x) > std::abs(velocity.y) && hit.normal.x != 0) {
                result = hit;
            } else if (std::abs(velocity.y) > std::abs(velocity.x) && hit.normal.y != 0) {
                result = hit;
            }
        }
    }

    return result;
}

void Physics::sweep_response( Body *body, const Vector2 &velocity )
{
    if (const Hit hit = sweep_static_bodies(body->mAABB, velocity); hit.mIsHit) {
        body->mAABB.mPos = hit.position;

        if (hit.normal.x != 0) {
            body->mAABB.mPos.y += velocity.y;
            body->mVelocity.x = 0;
        } else if (hit.normal.y != 0) {
            body->mAABB.mPos.x += velocity.x;
            body->mVelocity.y = 0;
        }
    } else {
        body->mAABB.mPos += velocity;
    }
}

void Physics::stationary_response( Body *body )
{
    for (const StaticBody &static_body: staticBodies) {
        AABB aabb = AABB::minkowski_difference(static_body.mAABB, body->mAABB);

        Vector2 min = aabb.min(), max = aabb.max();

        if (min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0) {
            body->mAABB.mPos += AABB::penetration_vector(aabb);
        }
    }
}

void Physics::update( const float deltaTime )
{
    for (Body &body: bodies) {
        body.mVelocity.y += gravity;
        if (body.mVelocity.y > terminalVelocity) {
            body.mVelocity.y = terminalVelocity;
        }

        body.mVelocity += body.mAcceleration;

        Vector2 scaled_velocity = body.mVelocity * (deltaTime * tickRate);

        for (u32 i = 0; i < iterations; ++i) {
            sweep_response(&body, scaled_velocity);
            stationary_response(&body);
        }
        body.mAABB.update_rect();
    }
}

Body *Physics::get_body( const usize index )
{
    return &bodies.at(index);
}

StaticBody *Physics::get_static_body( const usize index )
{
    return &staticBodies.at(index);
}

u32 Physics::add_body( const Vector2 &pos, const Vector2 &half_size, const bool filled )
{
    bodies.emplace_back(pos, half_size, filled);
    return bodies.size() - 1;
}

u32 Physics::add_static_body( const Vector2 &pos, const Vector2 &half_size, const bool filled )
{
    staticBodies.emplace_back(pos, half_size, filled);
    return staticBodies.size() - 1;
}
