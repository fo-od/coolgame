#include "player.hpp"

#include <utility>

#include "constants.hpp"
#include "globals.hpp"
#include "io.hpp"
#include "util.hpp"

Player::Player(Body body, const float speed, const float jumpHeight )
    : body(std::move(body)),
      speed(speed),
      jumpStrength(jumpHeight){}

void Player::update()
{
    // // TODO: implement collision detection
    // if ( SDL_HasRectIntersectionFloat(&rect, &WINDOW_RECT_BOTTOM) ) {
    //     while ( SDL_HasRectIntersectionFloat(&rect, &WINDOW_RECT_BOTTOM) ) {
    //         rect.y -= 0.1;
    //     }
    //     falling = false;
    // } else {
    //     falling = true;
    // }
    //
    // // TODO: add a dash timer
    // if ( falling ) {
    //     vel.x *= drag;
    //     if ( keyboardState[SDL_SCANCODE_DOWN] && !dashing ) {
    //         vel.y += jumpStrength;
    //         // dashing = true;
    //     }
    //     vel.y += gravity * deltaTime;
    // } else {
    //     vel.x *= friction;
    //     vel.y = 0;
    //     if ( keyboardState[SDL_SCANCODE_DOWN] && !dashing ) {
    //         vel.x += -( keyboardState[SDL_SCANCODE_LEFT] - keyboardState[SDL_SCANCODE_RIGHT] ) * speed / 2;
    //         // dashing = true;
    //     } else if ( keyboardState[SDL_SCANCODE_UP] ) {
    //         vel.y += -jumpStrength;
    //     }
    // }
    //
    // vel.x += -( keyboardState[SDL_SCANCODE_LEFT] - keyboardState[SDL_SCANCODE_RIGHT] ) * speed * deltaTime;
    //
    // rect.x += vel.x;
    // rect.y += vel.y;
}

void Player::draw() const
{
    // U_SetRenderDrawColor(COLOR_WHITE);
    // SDL_RenderFillRect(renderer, &rect);
    //
    // U_SetRenderDrawColor(COLOR_BLACK);
}
