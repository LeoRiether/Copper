#include "Sound.h"

#include "Resources.h"

#define MODULE "Sound"

Sound::Sound(GameObject& associated)
    : Component(associated), chunk(nullptr), channel(-1) {}

Sound::Sound(GameObject& associated, const string& file) : Sound(associated) {
    Open(file);
}

Sound::~Sound() {
    if (chunk) Stop();
}

void Sound::Play(int times) {
    if (chunk) {
        channel = Mix_PlayChannel(-1, chunk, times - 1);
    }
}

void Sound::Stop() {
    if (chunk && channel != -1) {
        Mix_HaltChannel(channel);
        channel = -1;
    }
}

void Sound::Open(const string& file) { chunk = Resources::Sound(file); }

bool Sound::IsOpen() { return chunk != nullptr; }

bool Sound::IsPlaying() { return channel != -1 && Mix_Playing(channel); }

void Sound::Update(float) {}
void Sound::Render(Vec2) {}
bool Sound::Is(CType type) { return type == CType::Sound; }
