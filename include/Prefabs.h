#pragma once

#include <string>

#include "GameObject.h"
#include "SDL_include.h"

GameObject* MakePlayer();
GameObject* MakeCompanion();
GameObject* MakeControlsTutorial();
GameObject* MakeEnemyFollower();
GameObject* MakeEnemyDistancer();
GameObject* MakeBarril();
GameObject* MakeEscavadeira();
GameObject* MakeBullet(Vec2<Cart> center, float angle);
GameObject* MakePlayerBullet(Vec2<Cart> center, float angle);
GameObject* MakeVigaB();
GameObject* MakeExplosion1();
GameObject* MakeExplosion3(Vec2<Cart> scale = Vec2<Cart>{1,1});
GameObject* MakeExplosion4();
GameObject* MakeStageTransitionDimmer_FadeIn();
GameObject* MakeStageTransitionDimmer_FadeOut();
GameObject* MakeOneOffAudio(std::string file, int volume = MIX_MAX_VOLUME);
GameObject* MakeSlash(Vec2<Cart> center, float angle);
vector<GameObject*> MakeMap1Colliders();
GameObject* MakeDialog(std::string dialogFile);
GameObject* MakeDialogTrigger(Rect base, std::string dialogFile);
GameObject* MakeLifeBar();
GameObject* MakeRandomPowerup();
GameObject* MakeHitMarker(int dmg);
GameObject* MakeCoin();
