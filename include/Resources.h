#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "SDL_include.h"
#include "wrap/Font.h"
#include "wrap/MixMusic.h"
#include "wrap/SoundChunk.h"
#include "wrap/Texture.h"

// TODO: write my own, faster, hashmap
template <class K, class V>
using hashmap = std::unordered_map<K, V>;

using std::shared_ptr;
using std::string;

inline string fontkey(const string& file, int ptsize) {
    return file + "@" + std::to_string(ptsize);
}

class Resources {
   private:
    static hashmap<string, shared_ptr<Texture>> images;
    static hashmap<string, shared_ptr<MixMusic>> music;
    static hashmap<string, shared_ptr<SoundChunk>> sounds;
    static hashmap<string, shared_ptr<MyFont>> fonts;

   public:
    static shared_ptr<Texture> Image(const string& file);
    static shared_ptr<MixMusic> Music(const string& file);
    static shared_ptr<SoundChunk> Sound(const string& file);
    static shared_ptr<MyFont> Font(const string& file, int ptsize);

    static void ClearImages();
    static void ClearMusic();
    static void ClearSounds();
    static void ClearFonts();
    static void ClearAll();
};
