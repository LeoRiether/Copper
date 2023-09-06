#pragma once

#include <string>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"

using std::string;

class Face : Component {
   private:
    int hitpoints = 30;

   public:
    Face(GameObject& associated);
    ~Face();

    void Damage(int damage);

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
};
