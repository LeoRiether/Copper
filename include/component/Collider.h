#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "math/Rect.h"
#include "math/Vec2.h"

class Collider : public Component {
private:
  Vec2<Cart> scale, offset;

public:
  Collider(GameObject &associated, Vec2<Cart> scale = {1, 1},
           Vec2<Cart> offset = {0, 0});

  Rect box;

  void Update(float dt);
  void Render(Vec2<Cart> camera);
  bool Is(CType type);
  void SetScale(Vec2<Cart> scale);
  void SetOffset(Vec2<Cart> offset);
};
