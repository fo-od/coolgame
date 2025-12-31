#pragma once

enum class Anchor : uint16_t
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
    return static_cast<Anchor>(static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs));
}

constexpr bool operator &( const Anchor lhs, const Anchor rhs )
{
    return static_cast<bool>(static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs));
}