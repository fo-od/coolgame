#pragma once

#include <vector>

#include "vector2.hpp"

#include "SDL3/SDL_rect.h"

class AABB
{
    static std::vector< SDL_FRect * > rects;
    static std::vector< SDL_FRect * > filled_rects;
    bool visible{};
    bool filled{};

    public:
        SDL_FRect rect{};
        Vector2 position{};
        Vector2 half_size{};

        AABB() = default;

        AABB( const Vector2 &position, const Vector2 &half_size, bool filled = false, bool visible = true );

        AABB( float x, float y, float half_width, float half_height );

        ~AABB();

        void update_rect();

        void show();

        void hide();

        static void draw();

        [[nodiscard]] Vector2 min() const;

        [[nodiscard]] Vector2 max() const;

        [[nodiscard]] AABB minkowski_difference( const AABB &other ) const;

        [[nodiscard]] bool intersects( const AABB &other ) const;

        [[nodiscard]] bool intersects( float x1, float y1, float x2, float y2 ) const;

        [[nodiscard]] Vector2 penetration_vector( const AABB &other ) const;
};
