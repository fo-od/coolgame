#pragma once

#include <SDL3/SDL_rect.h>
#include <stdexcept>

class Vector2
{
    public:
        float x = 0;
        float y = 0;

        Vector2() = default;

        Vector2( const float x, const float y ) : x(x),
                                                  y(y) {}

        Vector2( const int x, const int y ) : x(static_cast< float >(x)),
                                              y(static_cast< float >(y)) {}

        Vector2( const Vector2& other ) = default;

        ~Vector2() = default;


        [[nodiscard]] float get( const int val ) const
        {
            if ( val == 0 )
            {
                return x;
            }
            if ( val == 1 )
            {
                return y;
            }
            throw std::out_of_range("Invalid index");
        }


        constexpr Vector2 &operator =( const SDL_FPoint& rhs )
        {
            x = rhs.x;
            y = rhs.y;
            return *this;
        }

        constexpr bool operator ==( const Vector2& rhs ) const
        {
            return x == rhs.x && y == rhs.y;
        }

        // --- Multiplication ---

        constexpr Vector2 &operator *=( const Vector2& rhs )
        {
            x *= rhs.x;
            y *= rhs.y;
            return *this;
        }

        constexpr Vector2 &operator *=( const float& rhs )
        {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        constexpr Vector2 operator *( const float& rhs ) const
        {
            return Vector2{x * rhs, y * rhs};
        }

        // --- Addition ---

        constexpr Vector2 &operator +=( const Vector2& rhs )
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        constexpr Vector2 &operator +=( const float& rhs )
        {
            x += rhs;
            y += rhs;
            return *this;
        }

        constexpr Vector2 operator +( const Vector2& rhs ) const
        {
            return Vector2{x + rhs.x, y + rhs.y};
        }

        constexpr Vector2 operator +( const float& rhs ) const
        {
            return Vector2{x + rhs, y + rhs};
        }

        // --- Subtraction ---

        constexpr Vector2 &operator -=( const Vector2& rhs )
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        constexpr Vector2 &operator -=( const float& rhs )
        {
            x -= rhs;
            y -= rhs;
            return *this;
        }

        constexpr Vector2 operator -( const Vector2& rhs ) const
        {
            return Vector2{x - rhs.x, y - rhs.y};
        }

        constexpr Vector2 operator -( const float& rhs ) const
        {
            return Vector2{x - rhs, y - rhs};
        }
};
