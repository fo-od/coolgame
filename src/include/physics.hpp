#pragma once

#include <vector>

#include "body.hpp"

class Physics
{
    public:
        static std::vector< SDL_FRect * > rects;
        static std::vector< Body > bodies;

        static void update();

        static void add_body( const Vector2 &position, const Vector2 &size,
                              const Vector2 &velocity = Vector2{},
                              const Vector2 &acceleration = Vector2{} );
};
