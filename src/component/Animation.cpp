#include "component/Animation.h"

#include "GameObject.h"
#include "component/Sprite.h"
#include "util.h"

#define MODULE "Animation"

Animation::Animation(GameObject& associated) : Component(associated) {}

Animation* Animation::horizontal(GameObject& associated, Sprite& sprite,
                                 int frames, float frameTime) {
    if (frames <= 0) {
        fail2("can't create Animation with " BLUE "%d" RESET " frames", frames);
    }

    Animation* self = new Animation{associated};

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

void Animation::Play(const string& animationName) {
    auto id = nameToId.find(animationName);
    if (id == nameToId.end()) {
        fail2("animation " YELLOW "%s" RESET " does not exist!",
              animationName.c_str());
    }

    currentAnimation = id->second;
    currentFrame = 0;
    Update(0);
}

void Animation::Update(float) {
    auto& anim = animations[currentAnimation];

    if (timer.Get() >= anim[currentFrame].frameTime) {
        timer.Restart();
        currentFrame++;
        if (currentFrame >= (int)anim.size()) {
            currentFrame = 0;
        }
    }

    auto& frame = anim[currentFrame];
    auto sprite = (Sprite*)associated.GetComponent(CType::Sprite);
    if (sprite == nullptr)
        fail("no Sprite component found on the associated component");
    sprite->SetClip(frame.clipRect);

    associated.box.w = frame.clipRect.w;
    associated.box.h = frame.clipRect.h;
}

bool Animation::Is(CType type) { return type == CType::Animation; }

void Animation::Log() {
    log2("[%d] frame = %d", currentAnimation, currentFrame);
}
