#include "Timer.h"

#include "Game.h"

Timer::Timer() : startTime(Game::Instance().FrameStart()) {}

void Timer::Restart() { startTime = Game::Instance().FrameStart(); }

void Timer::Delay(float t) { startTime -= round(t * 1000.0f); }

float Timer::Get() {
  return (Game::Instance().FrameStart() - startTime) / 1000.0f;
}
