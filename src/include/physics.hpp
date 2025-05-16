#pragma once

#include <vector>

#include "body.hpp"
#include "types.hpp"
#include "vector.hpp"

class Physics
{
    static std::vector< Body > bodies;

    public:
        static void update();

        static void add_body( const Vector2 &pos, const Vector2 &size );

        static Body *get_body( usize index );

        static void draw();
};
