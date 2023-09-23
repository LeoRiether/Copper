#include "component/Text.h"

#include <algorithm>

#include "Game.h"
#include "Resources.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "util.h"

#define MODULE "Text"

Text::Text(GameObject& associated, string fontFile, int fontSize,
           TextStyle style, string text, SDL_Color color)
    : Component(associated),
      font(nullptr),
      texture(nullptr),
      text(std::move(text)),
      style(style),
      fontFile(std::move(fontFile)),
      fontSize(fontSize),
      color(color) {
    RemakeTexture();
}

Text::~Text() {}

void Text::Update(float) {}

void Text::Render(Vec2 camera) {
    auto& game = Game::Instance();
    SDL_Rect clipRect{0, 0, (int)associated.box.w, (int)associated.box.h};
    SDL_Rect destRect{int(associated.box.x - camera.x),
                      int(associated.box.y - camera.y), clipRect.w, clipRect.h};
    SDL_RenderCopyEx(game.Renderer(), texture->inner, &clipRect, &destRect, 0.0,
                     nullptr, SDL_FLIP_NONE);
}

bool Text::Is(CType type) { return type == CType::Text; }

void Text::SetText(string t) {
    text = std::move(t);
    RemakeTexture();
}
void Text::SetColor(SDL_Color c) {
    color = c;
    RemakeTexture();
}
void Text::SetStyle(TextStyle s) {
    style = s;
    RemakeTexture();
}
void Text::SetFontFile(string ff) {
    fontFile = std::move(ff);
    RemakeTexture();
}
void Text::SetFontSize(int fs) {
    fontSize = fs;
    RemakeTexture();
}

void Text::RemakeTexture() {
    font = Resources::Font(fontFile, fontSize);

    SDL_Surface* surface;
    switch (style) {
        case Text::Solid: {
            surface = TTF_RenderUTF8_Solid(font->inner, text.c_str(), color);
            break;
        }
        case Text::Shaded: {
            surface = TTF_RenderUTF8_Shaded(font->inner, text.c_str(), color,
                                            SDL_Color{0, 0, 0, 0});
            break;
        }
        case Text::Blended: {
            surface = TTF_RenderUTF8_Blended(font->inner, text.c_str(), color);
            break;
        }
    }

    texture = unique_ptr<Texture>(new Texture{
        SDL_CreateTextureFromSurface(Game::Instance().Renderer(), surface)});
    SDL_FreeSurface(surface);

    if (color.a != 255) {
        SDL_SetTextureAlphaMod(texture->inner, color.a);
    }

    int width, height;
    int queryStatus =
        SDL_QueryTexture(texture->inner, nullptr, nullptr, &width, &height);
    if (queryStatus != 0)
        fail2("couldn't query texture for font " YELLOW "%s@%d" RESET "",
              fontFile.c_str(), fontSize);

    associated.box.w = width;
    associated.box.h = height;
}