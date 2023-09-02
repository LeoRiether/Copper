#include "Music.h"

#include "util.h"

#define MODULE "Music"

Music::Music() : music(nullptr) {}

Music::Music(const string& file) {
    info2("loading music %s", file.c_str());
    Open(file);
}

Music::~Music() {
    if (music) {
        Stop(0);
        Mix_FreeMusic(music);
    }
}

void Music::Play(int times) {
    if (!music) {
        warn("playing <null> music?");
        return;
    }
    if (Mix_PlayMusic(music, times)) {
        sdlfail("Mix_PlayMusic failed");
    }
}

void Music::Stop(int msToStop) { Mix_FadeOutMusic(msToStop); }

void Music::Open(const string& file) {
    music = Mix_LoadMUS(file.c_str());
    if (!music)
        fail2("couldn't open music " YELLOW "'%s'" RESET "!", file.c_str());
}

bool Music::IsOpen() { return music != nullptr; }
