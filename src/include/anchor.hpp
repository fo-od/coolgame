#pragma once

enum class Anchor : u8
{
    None = 0,
    Left = 1 << 0,
    Right = 1 << 1,
    Center = 1 << 2,
    Top = 1 << 3,
    Bottom = 1 << 4
};

constexpr Anchor operator |( const Anchor lhs, const Anchor rhs )
{
    return static_cast<Anchor>(static_cast<u8>(lhs) | static_cast<u8>(rhs));
}

constexpr bool operator &( const Anchor lhs, const Anchor rhs )
{
    return static_cast<bool>(static_cast<u8>(lhs) & static_cast<u8>(rhs));
}
