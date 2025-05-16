#include "include/vector.hpp"

Vector2::Vector2( const float x, const float y )
    : x(x),
      y(y) {}

Vector2::Vector2( const Vector2 &other ) = default;

// Multiplication

Vector2 &Vector2::operator*=( const float op )
{
    x *= op;
    y *= op;
    return *this;
}

Vector2 Vector2::operator*( const float op ) const
{
    return Vector2{x * op, y * op};
}

// Addition

Vector2 &Vector2::operator+=( const Vector2 &op )
{
    x += op.x;
    y += op.y;
    return *this;
}
