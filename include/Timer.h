#pragma once

#include <cstdint>

class Timer {
private:
  int64_t startTime;

public:
  Timer();

  void Restart();
  void Delay(float t);
  float Get();
};
