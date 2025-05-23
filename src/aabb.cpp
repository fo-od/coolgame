#include "aabb.hpp"

#include "globals.hpp"

#include "SDL3/SDL.h"

std::vector< SDL_FRect * > AABB::rects;
std::vector< SDL_FRect * > AABB::filled_rects;

AABB::AABB( const Vector2 &position, const Vector2 &half_size, const bool filled, const bool visible )
    : visible(visible),
      filled(filled),
      rect(position.x + half_size.x,
           position.y - half_size.y,
           half_size.x * 2,
           half_size.y * 2),
      position(position),
      half_size(half_size)
{
    update_rect();
    if ( visible ) {
        if ( filled ) {
            filled_rects.push_back(&rect);
        } else {
            rects.push_back(&rect);
        }
    }
}

AABB::AABB( const float x, const float y, const float half_width, const float half_height )
    : AABB(Vector2{x, y}, Vector2{half_width, half_height}) {}

AABB::AABB( const AABB &other )
    : visible(other.visible),
      filled(other.filled),
      rect(other.rect),
      position(other.position),
      half_size(other.half_size)
{
    update_rect();
    if ( visible ) {
        if ( filled )
            filled_rects.push_back(&rect);
        else
            rects.push_back(&rect);
    }
}

AABB::~AABB() { hide(); }

void AABB::update_rect()
{
    rect.x = position.x;
    rect.y = position.y;
}

void AABB::show()
{
    if ( !visible ) {
        if ( filled ) {
            filled_rects.push_back(&rect);
        } else {
            rects.push_back(&rect);
        }
        visible = true;
    }
}

void AABB::hide()
{
    if ( visible ) {
        auto removeRect = [this]( std::vector< SDL_FRect * > &vec ) {
            if ( const auto it = std::find(vec.begin(), vec.end(), &rect); it != vec.end() ) {
                vec.erase(it);
                return true;
            }
            return false;
        };

        if ( !removeRect(rects) )
            removeRect(filled_rects);

        visible = false;
    }
}

void AABB::draw()
{
    if ( !filled_rects.empty() )
        SDL_RenderFillRects(renderer, *filled_rects.data(), filled_rects.size());
    if ( !rects.empty() )
        SDL_RenderRects(renderer, *rects.data(), rects.size());
}

Vector2 AABB::min() const { return position - half_size; }
Vector2 AABB::max() const { return position + half_size; }

AABB AABB::minkowski_difference( const AABB &other ) const
{
    return AABB{other.position - this->position, other.half_size + this->half_size};
}

bool AABB::intersects( const AABB &other ) const
{
    const AABB diff = minkowski_difference(other);
    const Vector2 min = diff.min(), max = diff.max();

    return min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0;
}

bool AABB::intersects( float x1, float y1, float x2, float y2 ) const {}

Vector2 AABB::penetration_vector( const AABB &other ) const
{
    Vector2 result;

    const AABB diff = minkowski_difference(other);
    const Vector2 min = diff.min(), max = diff.max();

    float min_dist = std::abs(min.x);
    result.x = min.x;
    result.y = 0;

    if ( std::abs(max.x) < min_dist ) {
        min_dist = std::abs(max.x);
        result.x = max.x;
    }

    if ( std::abs(min.y) < min_dist ) {
        min_dist = std::abs(min.y);
        result.x = 0;
        result.y = min.y;
    }

    if ( std::abs(max.y) < min_dist ) {
        result.x = 0;
        result.y = max.y;
    }

    return result;
}
