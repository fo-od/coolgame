#include "include/player.hpp"

#include "include/constants.hpp"
#include "include/globals.hpp"
#include "include/util.hpp"
#include "SDL3/SDL_events.h"

Player::Player( const float x, const float y, const float speed, const float gravity, const float jumpHeight,
                const float groundFriction, const float airFriction,
                const Vector2 &startingVelocity )
    : rect(x, y, 16, 16),
      vel(startingVelocity),
      speed(speed),
      gravity(gravity),
      jumpStrength(jumpHeight),
      groundFriction(groundFriction),
      airFriction(airFriction) {}

Player::Player( const float x, const float y, const float speed, const float gravity, const float jumpHeight,
                const float groundFriction, const float airFriction )
    : Player(x, y, speed, gravity, jumpHeight, groundFriction, airFriction, Vector2(0, 0)) {}


void Player::update()
{
    // TODO: implement collision detection

    if ( SDL_HasRectIntersectionFloat(&rect, &WINDOW_RECT_BOTTOM) ) {
        while ( SDL_HasRectIntersectionFloat(&rect, &WINDOW_RECT_BOTTOM) ) {
            rect.y -= 0.1;
        }
        falling = false;
    } else {
        falling = true;
    }

    // TODO: add a dash timer
    if ( falling ) {
        vel.x *= airFriction;
        if ( keyboardState[SDL_SCANCODE_DOWN] && !dashing ) {
            vel.y += jumpStrength;
            // dashing = true;
        }
        vel.y += gravity * deltaTime;
    } else {
        vel.x *= groundFriction;
        vel.y = 0;
        if ( keyboardState[SDL_SCANCODE_DOWN] && !dashing ) {
            vel.x += -( keyboardState[SDL_SCANCODE_LEFT] - keyboardState[SDL_SCANCODE_RIGHT] ) * speed / 2;
            // dashing = true;
        }
    }

    vel.x += -( keyboardState[SDL_SCANCODE_LEFT] - keyboardState[SDL_SCANCODE_RIGHT] ) * speed * deltaTime;
    if ( keyboardState[SDL_SCANCODE_UP] ) {
        vel.y += -jumpStrength * deltaTime;
    }

    rect.x += vel.x;
    rect.y += vel.y;
}

void Player::draw() const
{
    U_SetRenderDrawColor(COLOR_WHITE);
    SDL_RenderFillRect(renderer, &rect);

    U_SetRenderDrawColor(COLOR_BLACK);
}
