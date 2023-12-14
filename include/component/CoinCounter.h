#pragma once

#include "Component.h"

class CoinCounter : public Component {
   public:
	   CoinCounter(GameObject& go);
	   static CoinCounter* coinCounter;
	   void Render();

	   inline CType Key() const { return CType::CoinCounter; }

	   void AddCoins(int coins);

   private:
	   static int coins; 

};

