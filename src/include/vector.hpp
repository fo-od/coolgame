#pragma once

class Vector2
{
    public:
        double x, y;

        Vector2() = default;
        Vector2( double x, double y );
        ~Vector2() = default;

        Vector2( const Vector2 &other );

        Vector2& operator*=( double op );
};
