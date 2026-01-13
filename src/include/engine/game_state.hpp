#pragma once

#include "util/types.hpp"

enum class GameState : u8
{
    None    = 0,
    InMenu  = 1 << 0,
    Playing = 1 << 1,
    Editing = 1 << 2
};

constexpr GameState operator |( const GameState lhs, const GameState rhs )
{
    return static_cast< GameState >(static_cast< u8 >(lhs) | static_cast< u8 >(rhs));
}

constexpr GameState operator ^( const GameState lhs, const GameState rhs )
{
    return static_cast< GameState >(static_cast< u8 >(lhs) ^ static_cast< u8 >(rhs));
}

constexpr bool operator &( const GameState lhs, const GameState rhs )
{
    return static_cast< bool >(static_cast< u8 >(lhs) & static_cast< u8 >(rhs));
}