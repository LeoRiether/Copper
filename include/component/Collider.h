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

    tagset tags;
    Rect box;

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::Collider; }

    Collider* WithBase(Rect base);
    Collider* WithTag(int tag);
    Collider* ScaleToSprite();
};
