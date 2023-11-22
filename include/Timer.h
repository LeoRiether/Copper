#pragma once

#include <cstdint>

class Timer {
   private:
    double acc{0.0};

   public:
    Timer();

    void Update(double dt);
    void Restart();
    void Delay(double t);
    double Get();
};
