#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "math/Rect.h"
#include "math/Vec2.h"

class Collider : public Component {
   private:
    Rect base{0, 0, 0, 0};

   public:
    Collider(GameObject& associated);

    Rect box;

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);

    Collider* WithBase(Rect base);
    Collider* ScaleToSprite();
};
