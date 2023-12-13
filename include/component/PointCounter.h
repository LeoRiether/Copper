#pragma once

#include "Component.h"

class PointCounter : public Component {
   public:
	   PointCounter(GameObject& go);
	   static PointCounter* pointCounter;

	   inline CType Key() const { return CType::PointCounter; }

	   void AddPoints(int points);

   private:
	   static int points{0}; 

};

