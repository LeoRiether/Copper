#include <bitset>

using tagset = std::bitset<16>;
namespace tag {
constexpr int Terrain = 0;
constexpr int Player = 1;
constexpr int Entity = 2;
constexpr int Bullet = 3;
}  // namespace tag
