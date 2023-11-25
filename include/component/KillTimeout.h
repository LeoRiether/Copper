#pragma once

#include "Component.h"

class KillTimeout : public Component {
   public:
    KillTimeout(GameObject& go, float t);

    void Update(float dt);

    inline bool Is(CType type) { return type == CType::KillTimeout; }

   private:
    float timeout;
};
