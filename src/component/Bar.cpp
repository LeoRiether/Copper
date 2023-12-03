#include "component/Bar.h"

#include "component/Sprite.h"
#include "math/Rect.h"

#include <iostream>

Bar::Bar(GameObject &go, string file, int maxBar, Vec2<Cart> barDimension,
         int space)
    : Component(go) {
  bar = new Sprite(go, file);
  go.AddComponent(bar);
  this->maxBar = maxBar;
  this->space = space;
  barState = maxBar;
  this->barDimension = barDimension;
}

int Bar::GetBarState() { return barState; }

void Bar::SetBarState(int barState) {
  this->barState = barState;
  int x = barState * (barDimension.x + space);
  bar->SetClip(x, 0, barDimension.x, barDimension.y);
}