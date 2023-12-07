#pragma once

#include "Component.h"

class TextFadeIn : public Component {
   private:
    float fullFadeTimeS;
    float currentTime{0};

   public:
    TextFadeIn(GameObject& associated, float fullFadeTimeS);

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::TextFadeIn; }
};
