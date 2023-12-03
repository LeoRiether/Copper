#pragma once

#include "CType.h"
#include "Component.h"

class CameraFollower : public Component {
private:
  GameObject &go;
  Vec2<Cart> position;

public:
  CameraFollower(GameObject &go, Vec2<Cart> position = {0, 0});

  void Update(float dt);
  void Render(Vec2<Cart> camera);
  inline CType Key() const { return CType::CameraFollower; }
};
