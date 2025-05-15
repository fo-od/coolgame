#include "include/vector.hpp"

// Constructors
Vector2::Vector2( const float x, const float y )
    : x(x),
      y(y) {}

Vector2::Vector2( const Vector2 &other ) = default;

// Operators
Vector2 &Vector2::operator*=( const float op )
{
    x *= op;
    y *= op;
    return *this;
}
