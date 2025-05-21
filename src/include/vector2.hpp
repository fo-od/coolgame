#pragma once

class Vector2
{
    public:
        float x{}, y{};

        Vector2() = default;

        Vector2( float x, float y );

        Vector2( const Vector2 &other );

        ~Vector2() = default;

        void draw( float originX = {}, float originY = {} ) const;

        Vector2 &operator*=( float o );

        Vector2 operator*( float o ) const;

        Vector2 &operator+=( const Vector2 &o );

        Vector2 operator+( const Vector2 &o ) const;

        Vector2 operator-( const Vector2 &o ) const;
};
