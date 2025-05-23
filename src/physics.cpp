#include "physics.hpp"

#include "body.hpp"
#include "globals.hpp"

std::vector< Body > Physics::bodies;

void Physics::update()
{
    for ( Body &body : bodies ) {
        body.velocity += body.acceleration * deltaTime;
        body.aabb.position += body.velocity * deltaTime;
        body.aabb.update_rect();
    }
}

void Physics::add_body( const Vector2 &position, const Vector2 &size, const Vector2 &velocity,
                        const Vector2 &acceleration )
{
    bodies.emplace_back(AABB(position, size * 0.5), velocity, acceleration);
}