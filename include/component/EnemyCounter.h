#pragma once

#include "Component.h"
#include "Game.h"

class EnemyCounter : public Component {
   public:
	   EnemyCounter(GameObject& go, Vec2<Cart> pos = {SCREEN_WIDTH-127, 80});
	   void Update(float dt);

	   inline CType Key() const { return CType::EnemyCounter; }

};

