#pragma once

#include <deque>
#include <string>

#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"

using std::string;

class Text : public Component {
   public:  // /shrug
    enum TextStyle { Solid, Shaded, Blended };

   private:
    TTF_Font* font;
    SDL_Texture* texture;

    string text;
    TextStyle style;
    string fontFile;
    int fontSize;
    SDL_Color color;

   public:
    const char* debugName{"Text"};

    Text(GameObject& associated, string fontFile, int fontSize, TextStyle style,
         string text, SDL_Color color);
    ~Text();

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);

    inline SDL_Color Color() { return color; }

    void SetText(string text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontFile(string fontFile);
    void SetFontSize(int fontSize);

    void RemakeTexture();
};
