#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "SDL_include.h"

// TODO: write my own, faster, hashmap
template <class K, class V>
using hashmap = std::unordered_map<K, V>;

using std::string;

class Resources {
   private:
    static hashmap<string, SDL_Texture*> images;
    static hashmap<string, Mix_Music*> music;
    static hashmap<string, Mix_Chunk*> sounds;

   public:
    static SDL_Texture* Image(const string& file);
    static Mix_Music* Music(const string& file);
    static Mix_Chunk* Sound(const string& file);

    static void ClearImages();
    static void ClearMusic();
    static void ClearSounds();
};
