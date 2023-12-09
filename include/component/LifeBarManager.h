#pragma once

#include "Component.h"
#include "GameObject.h"
#include "component/Bar.h"

class LifeBarManager : public Component {
public:
  LifeBarManager(GameObject &go, int maxHP, Bar *lifeBar = nullptr);
  void Start(){};
  void Update(float dt);
  void Render(Vec2<Cart>){};
  inline CType Key() const { return CType::LifeBarManager; }
  void SetLifeBar(Bar *bar);
  // void SetEnergyBar(Bar *energyBar);
  void UpdateHP(int hp);

private:
  Bar *lifeBar;
  int maxHP;
  int prevHP;
  int prevLoss;
  // Bar *energyBar;
};
