#pragma once

#include "Component.h"
#include "GameObject.h"
#include "component/Sprite.h"

class RobotCan : public Component {
   public:
    RobotCan(GameObject& associated);

    void Update(float dt);

    bool Is(CType type);

   private:
    Sprite *coreSprite{nullptr}, *baseSprite{nullptr};
};
