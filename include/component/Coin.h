#pragma once

#include "Component.h"
#include "Game.h"

class Coin : public Component {
   public:
	   Coin(GameObject& go);

	   void Update(float dt);
	   void NotifyCollisionEnter(GameObject& other);

	   inline CType Key() const { return CType::Coin; }

   private:
	   bool moving{false};
	   bool collected{false};
	   Vec2<Cart> speed{500,500};
	   Vec2<Cart> target{SCREEN_WIDTH-120, 50};

};

