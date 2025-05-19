#pragma once

#include <vector>

#include "body.hpp"

class Physics
{
    static std::vector< SDL_FRect * > rects;

    public:
        static std::vector< Body > bodies;

        static void update();

        static void add_body( const Vector2 &position, const Vector2 &size,
                              const Vector2 &velocity = Vector2(0, 0),
                              const Vector2 &acceleration = Vector2(0, 0) );

        static void draw();
};
