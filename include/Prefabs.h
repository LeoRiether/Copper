#pragma once

#include <string>

#include "GameObject.h"
#include "SDL_include.h"

GameObject* MakePlayer();
GameObject* MakeEnemyFollower();
GameObject* MakeEnemyDistancer();
GameObject* MakeBarril();
GameObject* MakeEscavadeira();
GameObject* MakeBullet(Vec2<Cart> center, float angle);
GameObject* MakeVigaB();
GameObject* MakeExplosion1();
GameObject* MakeStageTransitionDimmer_FadeIn();
GameObject* MakeStageTransitionDimmer_FadeOut();
GameObject* MakeOneOffAudio(std::string file, int volume = MIX_MAX_VOLUME);
vector<GameObject*> MakeMap1Colliders();
