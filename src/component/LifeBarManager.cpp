#include "component/LifeBarManager.h"

#include "component/Player.h"

#define MODULE "LifeBarManager"

LifeBarManager::LifeBarManager(GameObject &go, int maxHP, Bar *lifeBar)
    : Component(go) {
  this->maxHP = maxHP;
  this->lifeBar = lifeBar;
  prevHP = 0;
  prevLoss = 0;
}

void LifeBarManager::SetLifeBar(Bar *bar) { lifeBar = bar; }

void LifeBarManager::Update(float dt) {
  int hp = Player::player->hp;
  if (hp != prevHP) {
    prevHP = hp;
    UpdateHP(hp);
  }
  int hpLoss = Player::player->hp;
  if (hpLoss != prevLoss) {
    prevLoss = hpLoss;
  }
}

void LifeBarManager::UpdateHP(int hp) {
  float hpUnit = maxHP / lifeBar->maxBar;
  lifeBar->SetBarState((int)hp / hpUnit);
}
