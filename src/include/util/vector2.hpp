#pragma once

#include <SDL3/SDL_rect.h>
#include <stdexcept>

class Vector2
{
    public:
        float x = 0;
        float y = 0;

        Vector2() = default;

        Vector2( const float x, const float y ) : x(x), y(y) {}

        Vector2( const int x, const int y ) : x(static_cast<float>(x)),
                                              y(static_cast<float>(y)) {}

        Vector2( const Vector2 &other ) = default;

        ~Vector2() = default;


        [[nodiscard]] float get( const int val ) const
        {
            if (val == 0) {
                return x;
            }
            if (val == 1) {
                return y;
            }
            throw std::invalid_argument("Invalid index");
        }


        Vector2 &operator=( const SDL_FPoint o )
        {
            x = o.x;
            y = o.y;
            return *this;
        }


        // --- Multiplication ---

        Vector2 &operator*=( const float o )
        {
            x *= o;
            y *= o;
            return *this;
        }

        Vector2 operator*( const float o ) const
        {
            return Vector2{x * o, y * o};
        }


        // --- Addition ---

        Vector2 &operator+=( const Vector2 &o )
        {
            x += o.x;
            y += o.y;
            return *this;
        }

        Vector2 operator+( const Vector2 &o ) const
        {
            return Vector2{x + o.x, y + o.y};
        }

        Vector2 operator+( const float o ) const
        {
            return Vector2{x + o, y + o};
        }


        // --- Subtraction ---

        Vector2 operator-( const Vector2 &o ) const
        {
            return Vector2{x - o.x, y - o.y};
        }

        Vector2 operator-( const float o ) const
        {
            return Vector2{x - o, y - o};
        }

        Vector2 &operator-=( const Vector2 &o )
        {
            x -= o.x;
            y -= o.y;
            return *this;
        }
};