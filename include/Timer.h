#pragma once

// Literally just a float
class Timer {
   private:
    float time{0};

   public:
    Timer();

    void Update(float dt);

    void Restart();
    float Get();
};
