#pragma once

#include <unordered_map>
#include <vector>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "Timer.h"
#include "component/Sprite.h"

using std::vector;

template <class K, class V>
using hashmap = std::unordered_map<K, V>;

struct Keyframe {
    SDL_Rect clipRect;
    float frameTime;
};

using Keyframes = vector<Keyframe>;

class Animation : public Component {
   private:
    Timer timer;
    vector<Keyframes> animations;
    hashmap<string, int> nameToId;
    int currentAnimation{0}, currentFrame{0};

    void AddKeyframes(const string& animationName, const Keyframes& kf);

   public:
    const char* DebugName() { return "Animation"; }

    Animation(GameObject& associated);

    /** Constructs an animation by dividing a spritesheet into `frames`
     * horizontal frames, in which all of them take `frameTime` to advance */
    static Animation* horizontal(GameObject& associated, Sprite& sprite,
                                 int frames, float frameTime);

    void Play(const string& animationName);

    void Update(float dt);
    inline void Render(Vec2<Cart>) {
        // it's the sprite that's rendered
    }
    bool Is(CType type);

    void Log();
};
