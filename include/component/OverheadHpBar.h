#pragma once

#include "Component.h"
#include "GameObject.h"

class OverheadHpBar : public Component {
   public:
    OverheadHpBar(GameObject& go, int initialHp, int maxHp);

    inline CType Key() const { return CType::OverheadHpBar; }

    void Update(float);
    void Render(Vec2<Cart> camera);

    inline int Hp() const { return hp; };
    inline int MaxHp() const { return maxHp; };
    inline void SetHp(int hp) { this->hp = hp; }
    inline void SetMaxHp(int maxHp) { this->maxHp = maxHp; }

   private:
    int hp{0}, maxHp{0};
    float shownHp{0};
};
