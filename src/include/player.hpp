#pragma once

#include "body.hpp"

class Player
{
    bool mFalling = false;
    bool mDashing = false;

    public:
        Body mBody{};
        float mSpeed{}, mJumpStrength{};

        Player() = default;

        Player( Body body, float speed, float jumpHeight );

        ~Player() = default;

        void update();

        void draw() const;
};