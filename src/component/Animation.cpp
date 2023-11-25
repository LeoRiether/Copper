#include "component/Animation.h"

#include "GameObject.h"
#include "component/Sprite.h"
#include "util.h"

#define MODULE "Animation"

Keyframe GridKeyframe::At(int x, int y) {
    const int frameWidth = sheetWidth / columns;
    const int frameHeight = sheetHeight / rows;
    return Keyframe{
        SDL_Rect{x * frameWidth, y * frameHeight, frameWidth, frameHeight},
        frameTime,
    };
}

Animation::Animation(GameObject& associated, Sprite& sprite)
    : Component(associated), sprite(sprite) {}

Animation* Animation::horizontal(GameObject& associated, Sprite& sprite,
                                 int frames, float frameTime) {
    if (frames <= 0) {
        fail2("can't create Animation with " BLUE "%d" RESET " frames", frames);
    }

    Animation* self = new Animation{associated, sprite};

    int frameWidth = sprite.SheetWidth() / frames;
    int frameHeight = sprite.SheetHeight();

    Keyframes kf(frames);
    for (int i = 0; i < frames; i++) {
        SDL_Rect clipRect{i * frameWidth, 0, frameWidth, frameHeight};
        kf[i] = Keyframe{clipRect, frameTime};
    }

    self->AddKeyframes("default", kf);

    // Can't call Play() here because there might not be an
    // associated.GetComponent(Sprite) btw
    self->currentAnimation = self->currentFrame = 0;
    sprite.SetClip(self->animations[0][0].clipRect);
    return self;
}

void Animation::AddKeyframes(const string& animationName, const Keyframes& kf) {
    int id = animations.size();
    nameToId[animationName] = id;
    animations.emplace_back(kf);
}

void Animation::Play(int id, bool looping) {
    currentAnimation = id;
    currentFrame = 0;
    loops = looping;
    Update(0);
}

void Animation::Play(const string& animationName, bool looping) {
    auto id = nameToId.find(animationName);
    if (id == nameToId.end()) {
        fail2("animation " YELLOW "%s" RESET " does not exist!",
              animationName.c_str());
    }
    Play(id->second, looping);
}

void Animation::SoftPlay(const string& animationName, bool looping) {
    auto id = nameToId.find(animationName);
    if (id == nameToId.end()) {
        fail2("animation " YELLOW "%s" RESET " does not exist!",
              animationName.c_str());
    }

    if (id->second != currentAnimation) Play(id->second, looping);
}

void Animation::Update(float dt) {
    auto& anim = animations[currentAnimation];

    timer.Update(dt);
    if (timer.Get() >= anim[currentFrame].frameTime) {
        timer.Restart();
        currentFrame++;
        if (currentFrame >= (int)anim.size()) {
            currentFrame = loops ? 0 : (int)anim.size() - 1;
        }
    }

    auto& frame = anim[currentFrame];
    sprite.SetClip(frame.clipRect);

    associated.box.w = frame.clipRect.w * sprite.Scale().x;
    associated.box.h = frame.clipRect.h * sprite.Scale().y;
}

bool Animation::Is(CType type) { return type == CType::Animation; }
