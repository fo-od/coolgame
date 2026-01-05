#pragma once

#include "engine/body.hpp"

class Player
{
    bool mFalling = false;
    bool mDashing = false;

    public:
        Body mBody;
        float mSpeed;
        float mJumpStrength;

        Player() = default;

        Player( Body body, float speed, float jumpHeight );

        ~Player() = default;

        void update();

        void draw() const;
};
