#pragma once

#include "Component.h"

class KillTimeout : public Component {
   public:
    KillTimeout(GameObject& go, float t);

    void Update(float dt);

    inline CType Key() const { return CType::KillTimeout; }

   private:
    float timeout;
};
