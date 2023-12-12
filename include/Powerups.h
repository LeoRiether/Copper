#pragma once

#include "GameObject.h"

class Powerups {
   public:
    enum Kind {
        DoubledBullets,
        MarioStar,
        Speedy,
        StrongerAttack,
    };

    int set{0};

    static void AddAnimatedSpriteTo(GameObject& go, int animation);

    float ApplyToBulletTimeout(float timeout);
    float ApplyToSpeed(float speed);
    float ApplyToAttackDamage(float damage);

   private:
    inline int ctz(int x) { return x ? __builtin_ctzll(x) : 32; }
    inline int next_bit(int bf, int bit) {
        return ctz(bf & ~((1UL << bit) - 1));
    }
};
