#pragma once

#include "Component.h"
#include "GameObject.h"

class TextBlinker : public Component {
   private:
    float fullBlinkTimeS;
    float currentTime{0};

   public:
    TextBlinker(GameObject& associated, float fullBlinkTimeS);

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);
};
