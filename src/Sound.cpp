#include "Sound.h"

#define MODULE "Sound"

Sound::Sound(GameObject& associated)
    : Component(associated), chunk(nullptr), channel(-1) {}

Sound::Sound(GameObject& associated, const string& file) : Sound(associated) {
    Open(file);
}

Sound::~Sound() {
    if (chunk) {
        Stop();
        Mix_FreeChunk(chunk);
    }
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

void Sound::Open(const string& file) {
    chunk = Mix_LoadWAV(file.c_str());
    if (!chunk) fail2("couldn't open " YELLOW "%s" RESET, file.c_str());
}

bool Sound::IsOpen() { return chunk != nullptr; }

void Sound::Update(float dt) { (void)dt; }
void Sound::Render() {}
bool Sound::Is(const string& type) { return type == "Sound"; }
