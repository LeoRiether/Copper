#pragma once

#include "Component.h"
#include "Game.h"

class EnemyCounter : public Component {
   public:
	   EnemyCounter(GameObject& go, bool gameData = false);
	   void Update(float dt);

	   inline CType Key() const { return CType::EnemyCounter; }

	   int enemyCount{0};
	   bool gameData;

};

