#pragma once

#include "SDL3/SDL_rect.h"

class Vector2
{
    public:
        float x = 0, y = 0;

        Vector2() = default;

        Vector2( float x, float y );

        Vector2( const Vector2 &other );

        ~Vector2() = default;

        void draw( float originX = 0, float originY = 0 ) const;

        void draw( const Vector2 &origin ) const;

        [[nodiscard]] float get( bool val ) const;

        Vector2 &operator=( SDL_FPoint o );

        Vector2 &operator*=( float o );

        Vector2 operator*( float o ) const;

        Vector2 &operator+=( const Vector2 &o );

        Vector2 operator+( const Vector2 &o ) const;

        Vector2 operator-( const Vector2 &o ) const;
};
