#include "include/player.hpp"

#include "include/constants.hpp"
#include "include/globals.hpp"
#include "include/util.hpp"
#include "SDL3/SDL_events.h"

Player::Player( const float x, const float y, const float speed, const float gravity, const float friction,
                const Vector2 &startingVelocity )
    : rect(x, y, 16, 16),
      vel(startingVelocity),
      speed(speed),
      gravity(gravity),
      friction(friction) {}

Player::Player( const float x, const float y, const float speed, const float gravity, const float friction )
    : Player(x, y, speed, gravity, friction, Vector2(0, 0)) {}

void Player::update()
{
    vel.x *= friction;
    // TODO: implement quadtrees for collision detection
    if ( false ) {
        vel.y += gravity * deltaTime;
    }

    vel.x += -( keyboardState[SDL_SCANCODE_LEFT] - keyboardState[SDL_SCANCODE_RIGHT] ) * speed * deltaTime;
    if ( keyboardState[SDL_SCANCODE_UP] ) {
        vel.y = -speed;
    }

    rect.x += vel.x;
    rect.y += vel.y;
}

void Player::draw() const
{
    U_SetRenderDrawColor(COLOR_WHITE);
    SDL_RenderRect(renderer, &rect);
    U_SetRenderDrawColor(COLOR_BLACK);
}
