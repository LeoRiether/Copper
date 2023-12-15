#pragma once

#include "Component.h"
#include "Game.h"

class CoinCounter : public Component {
   public:
	   CoinCounter(GameObject& go, Vec2<Cart> pos = {SCREEN_WIDTH-120, 50});
	   static CoinCounter* coinCounter;
	   void Update(float dt);

	   inline CType Key() const { return CType::CoinCounter; }

	   void AddCoin();

   private:
	   int target;

};

