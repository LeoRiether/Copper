#include "Music.h"

#include "Consts.h"
#include "Resources.h"
#include "util.h"

#define MODULE "Music"

Music::Music() : music(nullptr) {}

Music::Music(const string &file) {
    info2("loading music %s", file.c_str());
    Open(file);
}

Music::~Music() {
    if (music) Stop(0);
}

void Music::Play(int times) {
    if (!music) {
        warn("playing <null> music?");
        return;
    }
    static float &globalVolume = Consts::GetFloat("options.volume");
    if (globalVolume > 0.01 && Mix_PlayMusic(music->inner, times)) {
        sdlfail("Mix_PlayMusic failed");
    }
}

void Music::Stop(int msToStop) { Mix_FadeOutMusic(msToStop); }

void Music::Open(const string &file) { music = Resources::Music(file); }

bool Music::IsOpen() { return music != nullptr; }
