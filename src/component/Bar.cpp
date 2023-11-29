#include "component/Bar.h"

#include "component/Sprite.h"
#include "math/Rect.h"

#include <iostream>

Bar::Bar(GameObject &associated, string file, int maxBar,
         Vec2<Cart> barDimension, int space)
    : Component(associated) {
  bar = new Sprite(associated, file);
  associated.AddComponent(bar);
  this->maxBar = maxBar;
  this->space = space;
  barState = maxBar;
  this->barDimension = barDimension;

  teste = 0;
}

int Bar::GetBarState() { return barState; }

void Bar::SetBarState(int barState) {
  this->barState = barState;
  int x = barState * (barDimension.x + space);
  bar->SetClip(x, 0, barDimension.x, barDimension.y);
}

bool Bar::Is(CType type) { return type == CType::Bar; }

void Bar::Update(float dt) {
  int state = barState;
  std::cout << dt << std::endl;

  if (teste > 1) {
    std::cout << state << std::endl;
    if (state == 0) {
      SetBarState(6);
    } else {
      SetBarState(--barState);
    }
    teste = 0;
  }
  teste += dt;
}