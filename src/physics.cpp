#include "physics.hpp"

#include "body.hpp"
#include "globals.hpp"
#include "types.hpp"

std::vector< Body > Physics::bodies;
std::vector< StaticBody > Physics::static_bodies;

u32 Physics::iterations = 4;
float Physics::tick_rate = 1.0f / iterations;

float Physics::gravity = 10;
float Physics::terminal_velocity = 10000;


Hit Physics::sweep_static_bodies( const AABB &aabb, const Vector2 &velocity )
{
    Hit result{.time = 0xBEEF};

    for ( const StaticBody &static_body : static_bodies ) {
        AABB sum_aabb = static_body.aabb;
        sum_aabb.half_size += aabb.half_size;

        const Hit hit = sum_aabb.intersects(aabb.pos, velocity);
        if ( !hit.is_hit ) {
            continue;
        }

        if ( hit.time < result.time ) {
            result = hit;
        } else if ( hit.time == result.time ) {
            if ( std::abs(velocity.x) > std::abs(velocity.y) && hit.normal.x != 0 ) {
                result = hit;
            } else if ( std::abs(velocity.y) > std::abs(velocity.x) && hit.normal.y != 0 ) {
                result = hit;
            }
        }
    }

    return result;
}

void Physics::sweep_response( Body *body, const Vector2 &velocity )
{
    if ( const Hit hit = sweep_static_bodies(body->aabb, velocity); hit.is_hit ) {
        body->aabb.pos = hit.position;

        if ( hit.normal.x != 0 ) {
            body->aabb.pos.y += velocity.y;
            body->velocity.x = 0;
        } else if ( hit.normal.y != 0 ) {
            body->aabb.pos.x += velocity.x;
            body->velocity.y = 0;
        }
    } else {
        body->aabb.pos += velocity;
    }
}

void Physics::stationary_response( Body *body )
{
    for ( const StaticBody &static_body : static_bodies ) {
        AABB aabb = AABB::minkowski_difference(static_body.aabb, body->aabb);

        if ( Vector2 min = aabb.min(), max = aabb.max(); min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0 ) {
            const Vector2 penetration_vector = AABB::penetration_vector(aabb);
            body->aabb.pos += penetration_vector;
        }
    }
}

void Physics::update()
{
    for ( Body &body : bodies ) {
        body.velocity.y += gravity;
        if ( body.velocity.y > terminal_velocity ) {
            body.velocity.y = terminal_velocity;
        }

        body.velocity += body.acceleration;

        Vector2 scaled_velocity = body.velocity * deltaTime * tick_rate;

        for ( int i = 0; i < iterations; ++i ) {
            sweep_response(&body, scaled_velocity);
            stationary_response(&body);
        }
        body.aabb.update_rect();
    }
}

Body *Physics::get_body( const usize index )
{
    return &bodies.at(index);
}

StaticBody *Physics::get_static_body( const usize index )
{
    return &static_bodies.at(index);
}

u32 Physics::add_body( const Vector2 &pos, const Vector2 &half_size, const bool filled )
{
    bodies.emplace_back(pos, half_size, filled);
    return bodies.size() - 1;
}

u32 Physics::add_static_body( const Vector2 &pos, const Vector2 &half_size, const bool filled )
{
    static_bodies.emplace_back(pos, half_size, filled);
    return static_bodies.size() - 1;
}
