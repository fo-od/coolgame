#include "physics.hpp"

#include "body.hpp"
#include "globals.hpp"

void Physics::update()
{
    for ( Body &body : bodies ) {
        body.velocity += body.acceleration * deltaTime;
        body.aabb.position += body.velocity * deltaTime;
    }
}

void Physics::add_body( const Vector2 &pos, const Vector2 &size )
{
    bodies.push_back(Body(AABB(pos, size * 0.5), Vector2(0, 0), Vector2(0, 0)));
}

Body *Physics::get_body( const usize index )
{
    return &bodies.at(index);
}

void Physics::draw()
{
    for ( Body &body : bodies ) {
        // TODO: draw the bodies
    }
}
