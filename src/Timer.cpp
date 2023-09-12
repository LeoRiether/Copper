#include "Timer.h"

Timer::Timer() {}
void Timer::Update(float dt) { time += dt; }
void Timer::Restart() { time = 0; }
float Timer::Get() { return time; }
