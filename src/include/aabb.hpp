#pragma once

#include <vector>

#include "vector2.hpp"

#include "SDL3/SDL_rect.h"

class Hit
{
    public:
        bool is_hit = false;
        float time = -1;
        Vector2 position;
};

class AABB
{
    static std::vector< SDL_FRect * > rects;
    static std::vector< SDL_FRect * > filled_rects;
    bool visible = false;
    bool filled = false;

    public:
        SDL_FRect rect{};
        Vector2 pos{};
        Vector2 half_size{};

        AABB() = default;

        AABB( const Vector2 &position, const Vector2 &half_size, bool visible = true, bool filled = false );

        AABB( float x, float y, float half_width, float half_height );

        AABB( const AABB &other );

        ~AABB();

        void update_rect();

        void show();

        void hide();

        static void draw();

        [[nodiscard]] Vector2 min() const;

        [[nodiscard]] Vector2 max() const;

        [[nodiscard]] AABB minkowski_difference( const AABB &other ) const;

        [[nodiscard]] bool intersects( const AABB &other ) const;

        [[nodiscard]] Hit intersects( const Vector2& pos, const Vector2 &magnitude ) const;

        [[nodiscard]] Vector2 penetration_vector( const AABB &other ) const;
};
