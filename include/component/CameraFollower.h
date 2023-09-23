#pragma once

#include "CType.h"
#include "Component.h"

class CameraFollower : public Component {
   private:
    GameObject& go;

   public:
    const char* DebugName() { return "CameraFollower"; }

    CameraFollower(GameObject& go);

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
};