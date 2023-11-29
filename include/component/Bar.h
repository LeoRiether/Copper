#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "component/Sprite.h"
#include "math/Vec2.h"

#include <string>

using std::string;

class Bar : public Component {
public:
  int maxBar;

private:
  Sprite *bar;
  int barState;            // estado da barra (0 a maxBar)
  int space;               // espacamento entre barras
  Vec2<Cart> barDimension; // (w, h)
  float teste;

public:
  Bar(GameObject &associated, string file, int maxBar, Vec2<Cart> barDimension,
      int space);
  int GetBarState();
  void SetBarState(int barState);
  void Update(float dt);
  void Render(Vec2<Cart> camera){};
  bool Is(CType type);
};