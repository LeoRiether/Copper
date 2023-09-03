#pragma once

#include <string>

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
    void Render();
    bool Is(const string& type);
};
