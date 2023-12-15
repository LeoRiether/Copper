#pragma once

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include <string>

class DialogTrigger : public Component {
   public:
    DialogTrigger(GameObject& go, std::string dialogFile);

    inline CType Key() const { return CType::DialogTrigger; }

    void NotifyCollision(GameObject& other);
    void NotifyCollisionEnter(GameObject& other);
    void NotifyCollisionLeave(GameObject& other);

   private:
	std::string dialogFile{};
	bool collisionLastFrame{false};
};
