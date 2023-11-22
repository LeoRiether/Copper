#include "Timer.h"

Timer::Timer() : acc(0.0) {}

void Timer::Update(double dt) { acc += dt; }

void Timer::Delay(double dt) { acc -= dt; }

void Timer::Restart() { acc = 0; }

double Timer::Get() { return acc; }
