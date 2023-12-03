#pragma once

#include "GameObject.h"

GameObject *MakePlayer();
GameObject *MakeEnemyFollower();
GameObject *MakeEnemyDistancer();
GameObject *MakeBarril();
GameObject *MakeEscavadeira();
GameObject *MakeBullet(Vec2<Cart> center, float angle);
GameObject *MakeVigaB();
GameObject *MakeExplosion1();
vector<GameObject *> MakeMap1Colliders();
GameObject *MakeLifeBar();
