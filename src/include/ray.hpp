#pragma once

#include "vector2.hpp"

class Ray
{
    public:
        Vector2 position;
        Vector2 magnitude;

        Ray() = default;

        Ray( const Vector2 &position, const Vector2 &magnitude );

        Ray( float x1, float y1, float x2, float y2 );

        Ray( int x1, int y1, int x2, int y2 );

        void draw() const;
};
