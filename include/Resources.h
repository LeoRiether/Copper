#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "SDL_include.h"

// TODO: write my own, faster, hashmap
template <class K, class V>
using hashmap = std::unordered_map<K, V>;

using std::string;

inline string fontkey(const string& file, int ptsize) {
    return file + "@" + std::to_string(ptsize);
}

class Resources {
   private:
    static hashmap<string, SDL_Texture*> images;
    static hashmap<string, Mix_Music*> music;
    static hashmap<string, Mix_Chunk*> sounds;
    static hashmap<string, TTF_Font*> fonts;

   public:
    static SDL_Texture* Image(const string& file);
    static Mix_Music* Music(const string& file);
    static Mix_Chunk* Sound(const string& file);
    static TTF_Font* Font(const string& file, int ptsize);

    static void ClearImages();
    static void ClearMusic();
    static void ClearSounds();
    static void ClearFonts();
};
