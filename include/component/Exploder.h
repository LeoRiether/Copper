#pragma once

#include "Component.h"

class Exploder : public Component {
   public:
	Exploder(GameObject& go);

    inline CType Key() const { return CType::Exploder; }

	void NotifyCollision(GameObject&);

    Exploder* WithHp(int hp);

    void Die();
   private:
	int hp;

};

