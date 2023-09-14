#include "Resources.h"

#include "Game.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "util.h"

#define MODULE "Resources"

hashmap<string, SDL_Texture*> Resources::images;
hashmap<string, Mix_Music*> Resources::music;
hashmap<string, Mix_Chunk*> Resources::sounds;
hashmap<string, TTF_Font*> Resources::fonts;

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

TTF_Font* Resources::Font(const string& file, int ptsize) {
    auto it = fonts.find(fontkey(file, ptsize));
    if (it == fonts.end()) {
        log2("loading font %s", file.c_str());
        auto font = TTF_OpenFont(file.c_str(), ptsize);
        if (!font) fail2("couldn't open " YELLOW "%s@%d" RESET, file.c_str(), ptsize);
        it = fonts.emplace(fontkey(file, ptsize), font).first;
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

void Resources::ClearFonts() {
    for (auto [_key, font] : fonts) {
        TTF_CloseFont(font);
    }
    fonts.clear();
}
