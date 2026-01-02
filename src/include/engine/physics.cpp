#include "engine/physics.hpp"

#include "engine/body.hpp"
#include "util/types.hpp"
#include <cmath>
#include <ranges>
#include <unordered_map>

std::unordered_map<u32, Body> Physics::bodies;
std::unordered_map<u32, StaticBody> Physics::staticBodies;
u32 Physics::bodyCount = 0;

u32 Physics::iterations = 2;
float Physics::tickRate = 1.f / iterations;

float Physics::gravity = 100;
float Physics::terminalVelocity = 10000;


Hit Physics::sweep_static_bodies( const AABB &aabb, const Vector2 &velocity )
{
    Hit result{.mTime = INFINITY};

    for (const StaticBody &static_body: staticBodies | std::views::values) {
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
    for (const StaticBody &static_body: staticBodies | std::views::values) {
        AABB const aabb = AABB::minkowski_difference(static_body.mAABB, body->mAABB);

        Vector2 const min = aabb.min();
        Vector2 const max = aabb.max();

        if (min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0) {
            body->mAABB.mPos += AABB::penetration_vector(aabb);
        }
    }
}

void Physics::update( const float deltaTime )
{
    for (Body &body: bodies | std::views::values) {
        body.mVelocity.y += gravity;
        if (body.mVelocity.y > terminalVelocity) {
            body.mVelocity.y = terminalVelocity;
        }

        body.mVelocity += body.mAcceleration;

        Vector2 const scaled_velocity = body.mVelocity * (deltaTime * tickRate);

        for (u32 i = 0; i < iterations; ++i) {
            sweep_response(&body, scaled_velocity);
            stationary_response(&body);
        }
        body.mAABB.update_rect();
    }
}

Body *Physics::get_body( const u32 id )
{
    return &bodies.at(id);
}

StaticBody *Physics::get_static_body( const u32 id )
{
    return &staticBodies.at(id);
}

u32 Physics::add_body( const Vector2 &pos, const Vector2 &half_size, const bool filled )
{
    bodyCount++;
    bodies.try_emplace(bodyCount, pos, half_size, filled);
    return bodyCount;
}

u32 Physics::add_static_body( const Vector2 &pos, const Vector2 &half_size, const bool filled )
{
    bodyCount++;
    staticBodies.try_emplace(bodyCount, pos, half_size, filled);
    return bodyCount;
}