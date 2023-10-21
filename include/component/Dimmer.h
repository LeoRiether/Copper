#pragma once

#include "Component.h"
#include "GameObject.h"

class Dimmer : public Component {
public:
  Dimmer(GameObject &associated);

  void Update(float){};
  void Render(Vec2<Cart> camera);
  bool Is(CType type);
};
