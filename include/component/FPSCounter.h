#pragma once

#include "Component.h"
#include "GameObject.h"
#include "component/Text.h"

class FPSCounter : public Component {
   public:
    FPSCounter(GameObject& go);

    void Update(float dt);
    void Render(Vec2<Cart>);

    inline CType Key() const { return CType::FPSCounter; }

   private:
    Text* text{nullptr};

    constexpr static int N = 10;
    int i{0};
    double dts[N]{0};
    double sum{0};
};
