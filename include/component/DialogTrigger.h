#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include <string>

class DialogTrigger : public Component {
   public:
    DialogTrigger(GameObject& go, std::string dialogFile);

    inline CType Key() const { return CType::DialogTrigger; }

    void Update(float) {};
    void NotifyCollision(GameObject& other);

   private:
	std::string dialogFile{};
};
