#pragma once

#include <deque>
#include <optional>
#include <string>

#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "wrap/Font.h"
#include "wrap/Texture.h"

using std::shared_ptr;
using std::string;
using std::unique_ptr;

class Text : public Component {
   public:  // /shrug
    enum TextStyle { Solid, Shaded, Blended, Wrapped };

   private:
    shared_ptr<MyFont> font;
    unique_ptr<Texture> texture;

    string text;
    TextStyle style;
    string fontFile;
    int fontSize;
    SDL_Color color;
	int wrapWidth;

   public:
    Text(GameObject& associated, string fontFile, int fontSize, TextStyle style,
         string text, SDL_Color color, int wrapWidth = 0);
    ~Text();

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::Text; }

    inline SDL_Color Color() { return color; }

    void SetText(string text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontFile(string fontFile);
    void SetFontSize(int fontSize);
	void SetWrapWidth(int wrapWidth);

    void RemakeTexture();
};
