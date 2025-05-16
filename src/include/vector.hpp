#pragma once

class Vector2
{
    public:
        float x, y;

        Vector2() = default;
        Vector2( float x, float y );
        ~Vector2() = default;

        Vector2( const Vector2 &other );

        Vector2& operator*=( float op );
        Vector2 operator*( float op ) const;
        Vector2& operator+=( const Vector2 &op );
};
