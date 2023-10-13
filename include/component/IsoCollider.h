#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "math/Rect.h"
#include "math/Vec2.h"

class IsoCollider : public Component {
   private:
    Rect box;

   public:
    IsoCollider(GameObject& associated);

    Rect offset;

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);
};
