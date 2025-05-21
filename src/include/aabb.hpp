#pragma once

#include <vector>

#include "vector2.hpp"

#include "SDL3/SDL_rect.h"

class AABB
{
    static std::vector< SDL_FRect * > rects;

    public:
        SDL_FRect rect{};
        Vector2 position{};
        Vector2 half_size{};

        AABB() = default;

        AABB( const Vector2 &position, const Vector2 &half_size );

        ~AABB() = default;

        void update_rect();

        static void draw_rects();

        [[nodiscard]] Vector2 min() const;

        [[nodiscard]] Vector2 max() const;

        [[nodiscard]] AABB minkowski_difference( const AABB &other ) const;

        [[nodiscard]] bool intersects( const AABB &other ) const;

        [[nodiscard]] bool intersects( float x1, float y1, float x2, float y2 ) const;

        [[nodiscard]] Vector2 penetration_vector( const AABB &other ) const;
};
