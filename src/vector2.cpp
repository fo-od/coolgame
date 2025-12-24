#include "vector2.hpp"

Vector2::Vector2( const float x, const float y )
    : x(x),
      y(y) {}

Vector2::Vector2( const int x, const int y )
    : x(x),
      y(y) {}

Vector2::Vector2( const Vector2 &other ) = default;


float Vector2::get( const bool val ) const
{
    if (val) {
        return y;
    }
    return x;
}

Vector2 &Vector2::operator=( const SDL_FPoint o )
{
    x = o.x;
    y = o.y;
    return *this;
}


// Multiplication

Vector2 &Vector2::operator*=( const float o )
{
    x *= o;
    y *= o;
    return *this;
}

Vector2 Vector2::operator*( const float o ) const
{
    return Vector2{x * o, y * o};
}


// Addition

Vector2 &Vector2::operator+=( const Vector2 &o )
{
    x += o.x;
    y += o.y;
    return *this;
}

Vector2 Vector2::operator+( const Vector2 &o ) const
{
    return Vector2{x + o.x, y + o.y};
}

Vector2 Vector2::operator+( const float o ) const
{
    return Vector2{x + o, y + o};
}


// Subtraction

Vector2 Vector2::operator-( const Vector2 &o ) const
{
    return Vector2{x - o.x, y - o.y};
}

Vector2 Vector2::operator-( const float o ) const
{
    return Vector2{x - o, y - o};
}


Vector2 &Vector2::operator-=( const Vector2 &o )
{
    x -= o.x;
    y -= o.y;
    return *this;
}
