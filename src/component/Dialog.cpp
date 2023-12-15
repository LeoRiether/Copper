#include "component/Dialog.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "CType.h"
#include "Camera.h"
#include "Colors.h"
#include "Game.h"
#include "InputManager.h"
#include "Resources.h"
#include "component/Dimmer.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "limits.h"
#include "util.h"

#define MODULE "Dialog"

Dialog::Dialog(GameObject &go, std::string dialogFile)
    : Component(go), index(0) {
  script = DialogScript::fromFile(dialogFile);
  auto dimmerGo = new GameObject{};
  auto d = new Dimmer{*dimmerGo};
  dimmerGo->AddComponent(d);
  dimmerGo->box = go.box;

  dimmer = std::shared_ptr<GameObject>(dimmerGo);

  auto textboxGo = new GameObject{};
  auto sprite = new Sprite{*textboxGo, ASSETS "/img/Textbox.png"};
  textboxGo->AddComponent(sprite);
  textboxGo->box.x = (SCREEN_WIDTH - sprite->SheetWidth()) / 2;
  textboxGo->box.y = SCREEN_HEIGHT - 200 - sprite->SheetHeight() / 2;
  textbox = std::shared_ptr<GameObject>(textboxGo);

  float textboxX = textboxGo->box.x;
  float textboxY = textboxGo->box.y;
  Vec2<Cart> textboxScale = sprite->Scale();
  {
    auto msgGo = new GameObject{};
    auto text = new Text{*msgGo, ASSETS "/font/ROCK.TTF",
                         30,     Text::Wrapped,
                         ".",    colorFromHex("ffffff")};
    msgGo->AddComponent(text);
    text->SetWrapWidth(MSG_BOX_WIDTH);
    msgGo->box.x = MSG_BOX_X_OFFSET * textboxScale.x + textboxX;
    msgGo->box.y = MSG_BOX_Y_OFFSET * textboxScale.y + textboxY;
    msg = std::shared_ptr<GameObject>(msgGo);
  }

  {
    auto nameGo = new GameObject{};
    auto text = new Text{*nameGo, ASSETS "/font/AngelRhapsodyRegular-ZVGJz.ttf",
                         70,      Text::Blended,
                         ".",     colorFromHex("fdd17a")};
    nameGo->AddComponent(text);
    text->SetWrapWidth(NAME_BOX_WIDTH);
    nameGo->box = Rect{NAME_BOX_X_OFFSET * textboxScale.x + textboxX,
                       NAME_BOX_Y_OFFSET * textboxScale.y + textboxY,
                       NAME_BOX_WIDTH, NAME_BOX_HEIGHT};
    name = std::shared_ptr<GameObject>(nameGo);
  }

  auto speakerGo = new GameObject{};
  auto speakerSprite = new Sprite{*speakerGo, ASSETS "/img/Copper_d.png"};
  speakerGo->AddComponent(speakerSprite);
  speakerSprite->SetScale(0.2);
  speakerGo->box.SetTopLeft(Left);
  speaker = shared_ptr<GameObject>{speakerGo};
}

Dialog::~Dialog(){
  Game::Instance().Slowdown(1.0, 0);
  Game::Instance().GetState().playerMovement = true;
}

void Dialog::Start() {
  GetNextLine();
  Game::Instance().Slowdown(0.0, INFINITY);
  Game::Instance().GetState().playerMovement = false;
}

void Dialog::Update(float) {
  auto &input = InputManager::Instance();

  if (input.KeyPress(NEXT_KEY) || input.ControllerPress(SDL_CONTROLLER_BUTTON_A)) {
    if (index >= (int)script.size()) {
      associated.RequestDelete();
      return;
    }
	GetNextLine();
  }
}

void Dialog::Render(Vec2<Cart>) {
  dimmer->Render(Vec2<Cart>{0, 0});
  speaker->Render(Vec2<Cart>{0, 0});
  textbox->Render(Vec2<Cart>{0, 0});
  msg->Render(Vec2<Cart>{0, 0});
  name->Render(Vec2<Cart>{0, 0});
}

void Dialog::GetNextLine(){
  auto line = script[index++];
  ((Text *)msg->GetComponent(CType::Text))->SetText(line.content);
  ((Text *)name->GetComponent(CType::Text))->SetText(line.character);
  ((Sprite *)speaker->GetComponent(CType::Sprite))
      ->Open(ASSETS + std::string("/img/") + line.character + "_d.png");
  side = line.side;
  switch (side) {
	  case DialogLine::Left:
		  speaker->box.SetTopLeft(Left);
		  ((Sprite *)speaker->GetComponent(CType::Sprite))
			  ->fipHorizontal = false;
		  break;
	  case DialogLine::Right:
		  speaker->box.SetTopLeft(Right);
		  ((Sprite *)speaker->GetComponent(CType::Sprite))
			  ->fipHorizontal = true;
		  break;
  }
}
