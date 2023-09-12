#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "Rect.h"
#include "SDL_include.h"
#include "Vec2.h"

class Collider : public Component {
   private:
    Vec2 scale, offset;

   public:
    const char* DebugName() { return "Collider"; }

    Collider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});

    Rect box;

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);
};
