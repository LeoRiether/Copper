#include "Resources.h"

#include "Game.h"
#include "SDL_render.h"
#include "util.h"

#define MODULE "Resources"

hashmap<string, SDL_Texture*> Resources::images;
hashmap<string, Mix_Music*> Resources::music;
hashmap<string, Mix_Chunk*> Resources::sounds;

SDL_Texture* Resources::Image(const string& file) {
    auto it = images.find(file);
    if (it == images.end()) {
        log2("loading image %s", file.c_str());

        auto texture =
            IMG_LoadTexture(Game::Instance().Renderer(), file.c_str());
        if (!texture)
            fail2("couldn't open texture " YELLOW "'%s'" RESET "!",
                  file.c_str());

        it = images.emplace(file, texture).first;
    }
    return it->second;
}

Mix_Music* Resources::Music(const string& file) {
    auto it = music.find(file);
    if (it == music.end()) {
        log2("loading music %s", file.c_str());
        auto mus = Mix_LoadMUS(file.c_str());
        if (!mus)
            fail2("couldn't open music " YELLOW "'%s'" RESET "!", file.c_str());
        it = music.emplace(file, mus).first;
    }
    return it->second;
}

Mix_Chunk* Resources::Sound(const string& file) {
    auto it = sounds.find(file);
    if (it == sounds.end()) {
        log2("loading sound %s", file.c_str());
        auto chunk = Mix_LoadWAV(file.c_str());
        if (!chunk) fail2("couldn't open " YELLOW "%s" RESET, file.c_str());
        it = sounds.emplace(file, chunk).first;
    }
    return it->second;
}

void Resources::ClearImages() {
    for (auto [_file, image] : images) {
        SDL_DestroyTexture(image);
    }
    images.clear();
}

void Resources::ClearMusic() {
    for (auto [_file, mus] : music) {
        Mix_FreeMusic(mus);
    }
    music.clear();
}

void Resources::ClearSounds() {
    for (auto [_file, chunk] : sounds) {
        Mix_FreeChunk(chunk);
    }
    sounds.clear();
}