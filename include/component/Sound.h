#pragma once

#include <memory>
#include <string>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "util.h"
#include "wrap/SoundChunk.h"

using std::shared_ptr;
using std::string;

class Sound : public Component {
   private:
    shared_ptr<SoundChunk> chunk;
    int channel;

   public:
    Sound(GameObject& associated);
    Sound(GameObject& associated, const string& file);
    ~Sound();

    void Play(int times = 1);
    void Stop();
    void Open(const string& file);
    bool IsOpen();
    bool IsPlaying();

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::Sound; }
};
