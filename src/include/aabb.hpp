#pragma once

#include <vector>

#include "ray.hpp"
#include "vector2.hpp"

#include "SDL3/SDL_rect.h"

class Hit
{
    public:
        bool is_hit = false;
        float time = -1;
        Vector2 position;
        Vector2 normal;
};

class AABB
{
    static std::vector< SDL_FRect * > rects;
    static std::vector< SDL_FRect * > filled_rects;

    bool visible = false;
    bool filled = false;

    void removeRect() const;

    public:
        Vector2 pos{};
        Vector2 half_size{};
        SDL_FRect rect{};

        AABB() = default;

        AABB( const Vector2 &position, const Vector2 &half_size, bool visible = true, bool filled = false );

        AABB( float x, float y, float half_width, float half_height );

        AABB( const AABB &other );

        ~AABB();

        AABB &operator=( const AABB &other );

        static void draw();

        void update_rect();

        void show();

        void hide();

        [[nodiscard]] Vector2 min() const;

        [[nodiscard]] Vector2 max() const;

        static AABB minkowski_difference( const AABB &a, const AABB &b );

        [[nodiscard]] bool intersects( const AABB &other ) const;

        [[nodiscard]] Hit intersects( const Vector2 &pos, const Vector2 &magnitude ) const;

        [[nodiscard]] Hit intersects( const Ray &ray ) const;

        static Vector2 penetration_vector( const AABB &aabb );
};
