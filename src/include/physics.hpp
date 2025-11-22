#pragma once

#include <vector>

#include "body.hpp"
#include "types.hpp"

class Physics
{
    static std::vector<Body> bodies;
    static std::vector<StaticBody> staticBodies;

    static u32 iterations;
    static float tickRate;

    static float gravity;
    static float terminalVelocity;

    static Hit sweep_static_bodies( const AABB &aabb, const Vector2 &velocity );

    static void sweep_response( Body *body, const Vector2 &velocity );

    static void stationary_response( Body *body );

    public:
        static void update( float deltaTime );

        static Body *get_body( usize index );

        static StaticBody *get_static_body( usize index );

        static u32 add_body( const Vector2 &pos, const Vector2 &half_size, bool filled = false );

        static u32 add_static_body( const Vector2 &pos, const Vector2 &half_size, bool filled = false );
};