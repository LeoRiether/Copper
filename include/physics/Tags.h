#pragma once

#include <bitset>

using tagset = std::bitset<16>;
namespace tag {
inline constexpr int Terrain = 0;
inline constexpr int Player = 1;
inline constexpr int Entity = 2;
inline constexpr int Bullet = 3;
}  // namespace tag
