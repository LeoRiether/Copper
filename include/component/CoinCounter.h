#pragma once

#include "Component.h"

class CoinCounter : public Component {
   public:
	   CoinCounter(GameObject& go);
	   static CoinCounter* coinCounter;
	   void Update(float dt);

	   inline CType Key() const { return CType::CoinCounter; }

	   void AddCoin();

   private:
	   static int coins; 
	   static int target;

};

