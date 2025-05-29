#include "physics.hpp"

#include "body.hpp"
#include "globals.hpp"

std::vector< Body * > Physics::bodies;

void Physics::update()
{
    for ( Body *body : bodies ) {
        body->velocity += body->acceleration * deltaTime;
        body->aabb.pos += body->velocity * deltaTime;
        body->aabb.update_rect();
    }
}

void Physics::add_body( Body *body ) { bodies.push_back(body); }
