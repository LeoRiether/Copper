#pragma once

#include "Component.h"

class EnemyNop : public Component {
   public:
    void Update(float dt);
    inline CType Key() const { return CType::EnemyBehavior; }
};
