#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"

class EndOfStageTrigger : public Component {
   public:
    EndOfStageTrigger(GameObject& go);

    inline CType Key() const { return CType::EndOfStageTrigger; }

    void Update(float dt);
    void NotifyCollision(GameObject& other);

   private:
    bool active{false};
    float timer{0};
};
