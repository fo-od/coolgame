#include "aabb.hpp"

#include <ostream>
#include <cmath>

std::vector<SDL_FRect *> AABB::rects;
std::vector<SDL_FRect *> AABB::filledRects;

AABB::AABB( const Vector2 &position, const Vector2 &half_size, const bool visible, const bool filled )
    : mVisible(visible),
      mFilled(filled),
      mPos(position),
      mHalfSize(half_size),
      mRect(position.x - half_size.x,
            position.y - half_size.y,
            half_size.x * 2,
            half_size.y * 2)
{
    if (visible) {
        if (filled) {
            filledRects.push_back(&mRect);
        } else {
            rects.push_back(&mRect);
        }
    }
}

AABB::AABB( const float x, const float y, const float half_width, const float half_height )
    : mPos(x, y),
      mHalfSize(half_width, half_height) {}

AABB::AABB( const AABB &other )
    : mVisible(other.mVisible),
      mFilled(other.mFilled),
      mPos(other.mPos),
      mHalfSize(other.mHalfSize),
      mRect(other.mRect)
{
    if (mVisible) {
        if (mFilled)
            filledRects.push_back(&mRect);
        else
            rects.push_back(&mRect);
    }
}

AABB::~AABB()
{
    hide();
}

AABB &AABB::operator=( const AABB &other )
{
    mRect = other.mRect;
    mVisible = other.mVisible;
    mFilled = other.mFilled;
    mPos = other.mPos;
    mHalfSize = other.mHalfSize;

    if (mVisible) {
        if (mFilled) {
            filledRects.push_back(&mRect);
        } else {
            rects.push_back(&mRect);
        }
    }
    return *this;
}

void AABB::removeRect() const
{
    if (const auto r = std::find(rects.begin(), rects.end(), &mRect); r != rects.end()) {
        rects.erase(r);
    }
    if (const auto d = std::find(filledRects.begin(), filledRects.end(), &mRect); d != filledRects.end()) {
        filledRects.erase(d);
    }
}

void AABB::update_rect()
{
    mRect.x = mPos.x - mHalfSize.x;
    mRect.y = mPos.y - mHalfSize.y;
}

void AABB::show()
{
    if (mVisible) {
        return;
    }

    if (mFilled) {
        filledRects.push_back(&mRect);
    } else {
        rects.push_back(&mRect);
    }

    mVisible = true;
}

void AABB::hide()
{
    if (!mVisible) {
        return;
    }

    removeRect();

    mVisible = false;
}

void AABB::draw( SDL_Renderer *renderer )
{
    for (const SDL_FRect *rect: rects) {
        SDL_RenderRect(renderer, rect);
    }
    for (const SDL_FRect *rect: filledRects) {
        SDL_RenderFillRect(renderer, rect);
    }
}

Vector2 AABB::min() const
{
    return mPos - mHalfSize;
}

Vector2 AABB::max() const
{
    return mPos + mHalfSize;
}

AABB AABB::minkowski_difference( const AABB &a, const AABB &b )
{
    return AABB{a.mPos - b.mPos, a.mHalfSize + b.mHalfSize};
}

bool AABB::intersects( const AABB &other ) const
{
    const AABB diff = minkowski_difference(*this, other);
    const Vector2 min = diff.min();
    const Vector2 max = diff.max();

    return min.x <= 0 && max.x >= 0 && min.y <= 0 && max.y >= 0;
}

Hit AABB::intersects( const Vector2 &pos, const Vector2 &magnitude ) const
{
    Hit hit;
    const Vector2 min = this->min();
    const Vector2 max = this->max();

    float last_entry = -INFINITY;
    float first_exit = INFINITY;

    // repeat for 2 dimensions
    for (int i = 0; i < 2; i++) {
        // avoid divide by 0
        if (magnitude.get(i) != 0) {
            const float t1 = (min.get(i) - pos.get(i)) / magnitude.get(i);
            const float t2 = (max.get(i) - pos.get(i)) / magnitude.get(i);

            last_entry = std::max(last_entry, std::min(t1, t2));
            first_exit = std::min(first_exit, std::max(t1, t2));
        } else if (pos.get(i) <= min.get(i) || pos.get(i) >= max.get(i)) {
            return hit;
        }
    }

    if (first_exit > last_entry && first_exit > 0 && last_entry < 1) {
        hit.position = pos + magnitude * last_entry;
        hit.mIsHit = true;
        hit.mTime = last_entry;

        const float dx = hit.position.x - this->mPos.x,
                dy = hit.position.y - this->mPos.y;
        const float px = mHalfSize.x - std::abs(dx),
                py = mHalfSize.y - std::abs(dy);

        if (px < py) {
            hit.normal.x = (dx > 0) - (dx < 0);
        } else {
            hit.normal.y = (dy > 0) - (dy < 0);
        }
    }

    return hit;
}

Vector2 AABB::penetration_vector( const AABB &aabb )
{
    Vector2 result;

    const Vector2 min = aabb.min(),
            max = aabb.max();

    float min_dist = std::abs(min.x);
    result.x = min.x;
    result.y = 0;

    if (std::abs(max.x) < min_dist) {
        min_dist = std::abs(max.x);
        result.x = max.x;
    }

    if (std::abs(min.y) < min_dist) {
        min_dist = std::abs(min.y);
        result.x = 0;
        result.y = min.y;
    }

    if (std::abs(max.y) < min_dist) {
        result.x = 0;
        result.y = max.y;
    }

    return result;
}
