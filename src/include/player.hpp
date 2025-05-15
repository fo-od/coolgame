#pragma once

#include "body.hpp"

class Player
{
    bool falling = false;
    bool dashing = false;

    public:
        Body body{};
        float speed{}, jumpStrength{};

        Player() = default;

        Player( Body body, float speed, float jumpHeight );

        ~Player() = default;

        void update();

        void draw() const;
};
