#pragma once

#include "CType.h"
#include "Component.h"

class CameraFollower : public Component {
   private:
    GameObject& go;

   public:
    CameraFollower(GameObject& go);

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::CameraFollower; }
};
