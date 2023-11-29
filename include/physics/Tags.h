#pragma once

#include <bitset>

using tagset = std::bitset<16>;
namespace tag {
inline constexpr int Terrain = 0;
inline constexpr int VTerrain = 1;
inline constexpr int Player = 2;
inline constexpr int Entity = 3;
inline constexpr int Bullet = 4;
}  // namespace tag
