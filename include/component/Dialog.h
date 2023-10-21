#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "CType.h"
#include "Component.h"
#include "DialogScript.h"
#include "Game.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "Sprite.h"
#include "Text.h"
#include "math/Rect.h"
#include "math/Vec2.h"
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

class Dialog : public Component {
private:
  DialogScript script;
  shared_ptr<GameObject> speaker;
  shared_ptr<GameObject> msg;
  shared_ptr<GameObject> name;
  shared_ptr<GameObject> textbox;
  DialogLine::Side side;
  inline static Vec2<Cart> Left{200, 350};
  inline static Vec2<Cart> Right{SCREEN_WIDTH - 400, 350};
  int index;

public:
  Dialog(GameObject &go, string dialogFile);

  void Update(float dt);
  void Render(Vec2<Cart> camera);
  bool Is(CType type);
  void Start();
};
