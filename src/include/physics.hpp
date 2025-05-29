#pragma once

#include <vector>

#include "body.hpp"

class Physics
{
    static std::vector< Body * > bodies;

    public:
        static void update();

        static void add_body( Body *body );
};
