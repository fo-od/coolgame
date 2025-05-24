#include "aabb.hpp"

#include <ostream>

#include "globals.hpp"

#include "SDL3/SDL.h"

std::vector< SDL_FRect * > AABB::rects;
std::vector< SDL_FRect * > AABB::filled_rects;

AABB::AABB( const Vector2 &position, const Vector2 &half_size, const bool visible, const bool filled )
    : visible(visible),
      filled(filled),
      rect(position.x - half_size.x,
           position.y - half_size.y,
           half_size.x * 2,
           half_size.y * 2),
      pos(position),
      half_size(half_size)
{
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
      pos(other.pos),
      half_size(other.half_size)
{
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
    rect.x = pos.x - half_size.x;
    rect.y = pos.y - half_size.y;
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

Vector2 AABB::min() const { return pos - half_size; }
Vector2 AABB::max() const { return pos + half_size; }

AABB AABB::minkowski_difference( const AABB &other ) const
{
    return AABB{other.pos - this->pos, other.half_size + this->half_size};
}

bool AABB::intersects( const AABB &other ) const
{
    const AABB diff = minkowski_difference(other);
    const Vector2 min = diff.min(), max = diff.max();

    std::print(stdout, "{}\n", min.x);

    return min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0;
}

Hit AABB::intersects( const Vector2 &pos, const Vector2 &magnitude ) const
{
    Hit hit;
    const Vector2 min = this->min(), max = this->max();

    float last_entry = -INFINITY;
    float first_exit = INFINITY;

    for ( int i = 0; i < 2; i++ ) {
        if ( magnitude.get(i) != 0 ) {
            const float t1 = ( min.get(i) + pos.get(i) ) / magnitude.get(i);
            const float t2 = ( max.get(i) + pos.get(i) ) / magnitude.get(i);

            last_entry = std::max(last_entry, std::min(t1, t2));
            first_exit = std::min(first_exit, std::max(t1, t2));
        } else if ( pos.get(i) <= min.get(i) || pos.get(i) >= max.get(i) ) {
            return hit;
        }
    }

    if ( first_exit > last_entry && first_exit > 0 && last_entry < 1 ) {
        hit.position = pos + magnitude * last_entry;
        hit.is_hit = true;
        hit.time = last_entry;
    }
    return hit;
}

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
