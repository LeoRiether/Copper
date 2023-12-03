#include "component/LifeBarManager.h"

#define MODULE "LifeBarManager"

LifeBarManager::LifeBarManager(GameObject &go, int maxHP, Bar *lifeBar)
    : Component(go) {
  this->maxHP = maxHP;
  this->lifeBar = lifeBar;
}

void LifeBarManager::SetLifeBar(Bar *bar) { lifeBar = bar; }

void LifeBarManager::UpdateHP(int hp) {
  float hpUnit = maxHP / lifeBar->maxBar;
  lifeBar->SetBarState((int)hp / hpUnit);
}