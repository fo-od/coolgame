#pragma once

class Vector2
{
    public:
        float x{}, y{};

        Vector2() = default;

        Vector2( float originX, float originY );

        Vector2( const Vector2 &other );

        ~Vector2() = default;

        void draw( float x1 = {}, float y1 = {} ) const;

        Vector2 &operator*=( float o );

        Vector2 operator*( float o ) const;

        Vector2 &operator+=( const Vector2 &o );

        Vector2 operator+( const Vector2 &o ) const;

        Vector2 operator-( const Vector2 &o ) const;
};
