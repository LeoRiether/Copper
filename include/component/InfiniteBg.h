#pragma once

#include <memory>
#include <string>

#include "Component.h"
#include "wrap/Texture.h"
using std::shared_ptr;
using std::string;

class InfiniteBg : public Component {
private:
  shared_ptr<Texture> texture;
  int width, height;

public:
  InfiniteBg(GameObject &associated, const string &file);

  virtual void Update(float dt);
  virtual void Render(Vec2<Cart> camera);

  virtual bool Is(CType type);
};
