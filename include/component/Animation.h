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

/** Mostly just helps to build Keyframes from uniform grid sprites */
struct GridKeyframe {
    int columns, rows;
    int sheetWidth, sheetHeight;
    float frameTime;

    Keyframe At(int x, int y);
};

class Animation : public Component {
   private:
    Sprite& sprite;
    Timer timer{};
    vector<Keyframes> animations;
    hashmap<string, int> nameToId;
    int currentAnimation{0}, currentFrame{0};
    bool loops{true};

    /** Play animation from start, given the ID instead of name */
    void Play(int id, bool looping = true);

   public:
    Animation(GameObject& associated, Sprite& sprite);

    /** Constructs an animation by dividing a spritesheet into `frames`
     * horizontal frames, in which all of them take `frameTime` to advance */
    static Animation* horizontal(GameObject& associated, Sprite& sprite,
                                 int frames, float frameTime);

    /** Play animation from start */
    void Play(const string& animationName, bool looping = true);

    /** Play animation if it's not already playing */
    void SoftPlay(const string& animationName, bool looping = true);

    void AddKeyframes(const string& animationName, const Keyframes& kf);

    void Update(float dt);
    inline void Render(Vec2<Cart>) {
        // it's the sprite that's rendered
    }
    inline CType Key() const { return CType::Animation; }
};
