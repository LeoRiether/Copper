#pragma once

#include "Component.h"

class EnemyNop : public Component {
   public:
    void Update(float dt);
    inline bool Is(CType type) { return type == CType::EnemyBehavior; }
};
