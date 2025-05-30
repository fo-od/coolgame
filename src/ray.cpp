#include "include/ray.hpp"
#include "include/util.hpp"

Ray::Ray( const Vector2 &position, const Vector2 &magnitude )
    : position(position),
      magnitude(magnitude) {}

Ray::Ray( const float x1, const float y1, const float x2, const float y2 )
    : position(Vector2{x1, y1}),
      magnitude(Vector2{x2, y2}) {}

Ray::Ray( const int x1, const int y1, const int x2, const int y2 )
    : position(Vector2{x1, y1}),
      magnitude(Vector2{x2, y2}) {}

void Ray::draw() const
{
    U_RenderLine(position, magnitude);
}
