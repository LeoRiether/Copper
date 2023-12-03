#pragma once

#include "CType.h"
#include "Component.h"

class StageTransitionDimmer : public Component {
   public:
    enum Easing { In, Out };

    StageTransitionDimmer(GameObject& go, float from, float to, float duration);

    inline CType Key() const { return CType::StageTransitionDimmer; }

    StageTransitionDimmer* WithEasing(Easing e);

    void Update(float);
    void Render(Vec2<Cart>);

   private:
    float from{0}, to{0}, duration{0};
    Easing easing{In};

    float now{0};
};
