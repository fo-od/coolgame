#pragma once

#include <vector>

#include "body.hpp"
#include "types.hpp"
#include "vector.hpp"

class Physics
{
    std::vector<Body> bodies;

    public:
        Physics() = default;

        ~Physics() = default;

        void update();

        void add_body( const Vector2 &pos, const Vector2 &size );

        Body *get_body( usize index );
};
