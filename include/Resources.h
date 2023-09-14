#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "SDL_include.h"

// TODO: write my own, faster, hashmap
template <class K, class V>
using hashmap = std::unordered_map<K, V>;

using std::string;
using std::shared_ptr;

inline string fontkey(const string& file, int ptsize) {
    return file + "@" + std::to_string(ptsize);
}

class Resources {
   private:
    static hashmap<string, shared_ptr<SDL_Texture>> images;
    static hashmap<string, shared_ptr<Mix_Music>> music;
    static hashmap<string, shared_ptr<Mix_Chunk>> sounds;
    static hashmap<string, shared_ptr<TTF_Font>> fonts;

   public:
    static shared_ptr<SDL_Texture> Image(const string& file);
    static shared_ptr<Mix_Music> Music(const string& file);
    static shared_ptr<Mix_Chunk> Sound(const string& file);
    static shared_ptr<TTF_Font> Font(const string& file, int ptsize);

    static void ClearImages();
    static void ClearMusic();
    static void ClearSounds();
    static void ClearFonts();
};
