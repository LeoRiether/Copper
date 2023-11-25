#pragma once

#include "GameObject.h"

GameObject* MakePlayer();
GameObject* MakeEnemyFollower();
GameObject* MakeEnemyDistancer();
GameObject* MakeBarril();
GameObject* MakeEscavadeira();
GameObject* MakeBullet(Vec2<Cart> center, float angle);
vector<GameObject*> MakeMap1Colliders();
