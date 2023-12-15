#pragma once

#include "Music.h"
#include "state/State.h"

class EndState : public State {
private:
  Music bgMusic;
  int EnemyCount{0};
  float timer{0};
  bool leave = false;

public:
  EndState();
  ~EndState();

  void LoadAssets();
  void Update(float dt);
  void Render();

  void Start();
  void Pause();
  void Resume();
};
