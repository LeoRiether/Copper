#include "component/Sound.h"

#include "Resources.h"
#include "SDL_mixer.h"

#define MODULE "Sound"

Sound::Sound(GameObject& associated)
    : Component(associated), chunk(nullptr), channel(-1) {}

Sound::Sound(GameObject& associated, const string& file, int volume)
    : Sound(associated) {
    this->volume = volume;
    Open(file);
}

Sound::~Sound() {
    if (chunk) Stop();
}

void Sound::Play(int times) {
    if (chunk->inner) {
        channel = Mix_PlayChannel(-1, chunk->inner, times - 1);
        Mix_Volume(channel, volume);
    }
}

void Sound::Stop() {
    if (chunk->inner && channel != -1) {
        Mix_HaltChannel(channel);
        channel = -1;
    }
}

void Sound::Open(const string& file) { chunk = Resources::Sound(file); }

bool Sound::IsOpen() { return chunk != nullptr; }

bool Sound::IsPlaying() { return channel != -1 && Mix_Playing(channel); }

void Sound::Update(float) {}
void Sound::Render(Vec2<Cart>) {}
