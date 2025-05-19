#pragma once

#include <SDL3/SDL_rect.h>
#include "vector.hpp"

class AABB
{
    public:
        SDL_FRect rect{};
        Vector2 position{};
        Vector2 half_size{};

        AABB() = default;

        AABB( const Vector2 &position, const Vector2 &half_size );

        ~AABB() = default;

        void update_rect();

        [[nodiscard]] Vector2 min() const;

        [[nodiscard]] Vector2 max() const;

        [[nodiscard]] AABB minkowski_difference( const AABB &other ) const;

        [[nodiscard]] bool intersects( const AABB &other ) const;

        [[nodiscard]] Vector2 penetration_vector( const AABB &other ) const;
};
