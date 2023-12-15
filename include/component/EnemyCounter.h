#pragma once

#include "Component.h"

class EnemyCounter : public Component {
   public:
	   EnemyCounter(GameObject& go);
	   void Update(float dt);

	   inline CType Key() const { return CType::EnemyCounter; }

};

