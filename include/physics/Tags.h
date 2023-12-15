#pragma once

#include <bitset>

using tagset = std::bitset<16>;
namespace tag {
inline constexpr int Terrain = 0;
inline constexpr int VTerrain = 1;
inline constexpr int Player = 2;
// An enemy is an entity, but an entity might not be an enemy
inline constexpr int Entity = 3;
inline constexpr int Enemy = 4;
inline constexpr int Bullet = 5;
inline constexpr int PlayerHitbox = 6;
inline constexpr int EnemyHitbox = 7;
inline constexpr int Trigger = 8;
inline constexpr int Explosion = 9;
inline constexpr int Coin = 10;
}  // namespace tag
