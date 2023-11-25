#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "math/Rect.h"
#include "math/Vec2.h"
#include "physics/Tags.h"

class IsoCollider : public Component {
   public:
    IsoCollider(GameObject& associated);

    tagset tags;
    Rect base, box;

    void Update(float dt);
    void Render(Vec2<Cart> camera);

    IsoCollider* ScaleToSprite();
    IsoCollider* ExpandBy(float pixels);
    IsoCollider* WithBase(Rect b);

    bool Is(CType type);
};
