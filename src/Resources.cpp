#include "Resources.h"
#include <memory>

#include "Game.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "util.h"
#include "SDL_include.h"

#define MODULE "Resources"

hashmap<string, shared_ptr<SDL_Texture>> Resources::images;
hashmap<string, shared_ptr<Mix_Music>> Resources::music;
hashmap<string, shared_ptr<Mix_Chunk>> Resources::sounds;
hashmap<string, shared_ptr<TTF_Font>> Resources::fonts;

shared_ptr<SDL_Texture> Resources::Image(const string& file) {
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

shared_ptr<Mix_Music> Resources::Music(const string& file) {
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

shared_ptr<Mix_Chunk> Resources::Sound(const string& file) {
    auto it = sounds.find(file);
    if (it == sounds.end()) {
        log2("loading sound %s", file.c_str());
        auto chunk = Mix_LoadWAV(file.c_str());
        if (!chunk) fail2("couldn't open " YELLOW "%s" RESET, file.c_str());
        it = sounds.emplace(file, chunk).first;
    }
    return it->second;
}

shared_ptr<TTF_Font> Resources::Font(const string& file, int ptsize) {
    auto it = fonts.find(fontkey(file, ptsize));
    if (it == fonts.end()) {
        log2("loading font %s", file.c_str());
        auto font = TTF_OpenFont(file.c_str(), ptsize);
        if (!font)
            fail2("couldn't open " YELLOW "%s@%d" RESET, file.c_str(), ptsize);
        it = fonts.emplace(fontkey(file, ptsize), font).first;
    }
    return it->second;
}

void Resources::ClearImages() {
    vector<string> keysToDelete;
    for (auto& [file, image] : images) {
        if (image.unique()) {
            log2("freeing image %s", file.c_str());
            keysToDelete.push_back(file);
            SDL_DestroyTexture(image.get());
        }
    }
    for (auto& file : keysToDelete) {
        images.erase(file);
    }
}

void Resources::ClearMusic() {
    vector<string> keysToDelete;
    for (auto& [file, mus] : music) {
        if (mus.unique()) {
            log2("freeing music %s", file.c_str());
            keysToDelete.push_back(file);
            Mix_FreeMusic(mus.get());
        }
    }
    for (auto& file : keysToDelete) {
        music.erase(file);
    }
}

void Resources::ClearSounds() {
    vector<string> keysToDelete;
    for (auto& [file, chunk] : sounds) {
        if (chunk.unique()) {
            log2("freeing sound %s", file.c_str());
            keysToDelete.push_back(file);
            Mix_FreeChunk(chunk.get());
        }
    }
    for (auto& file : keysToDelete) {
        sounds.erase(file);
    }
}

void Resources::ClearFonts() {
    vector<string> keysToDelete;
    for (auto& [key, font] : fonts) {
        if (font.unique()) {
            log2("freeing font %s", key.c_str());
            keysToDelete.push_back(key);
            TTF_CloseFont(font.get());
        }
    }
    for (auto& key : keysToDelete) {
        fonts.erase(key);
    }
}
