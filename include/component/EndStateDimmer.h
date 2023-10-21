#pragma once

#include "Component.h"
#include "GameObject.h"

class EndStateDimmer : public Component {
private:
  float timer{0};
  float secondsToDeath;

public:
  EndStateDimmer(GameObject &associated, float secondsToDeath);

  void Update(float dt);
  void Render(Vec2<Cart> camera);
  bool Is(CType type);
};
