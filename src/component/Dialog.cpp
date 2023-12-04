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
  go.AddComponent(new Dimmer{go});

  auto textboxGo = new GameObject{};
  auto sprite = new Sprite{*textboxGo, ASSETS "/img/textbox.png"};
  textboxGo->AddComponent(sprite);
  textboxGo->box.x = (SCREEN_WIDTH - sprite->SheetWidth()) / 2;
  textboxGo->box.y = SCREEN_HEIGHT - 200 - sprite->SheetHeight() / 2;
  textbox = std::shared_ptr<GameObject>(textboxGo);

  float textboxX = textboxGo->box.x;
  float textboxY = textboxGo->box.y;
  Vec2<Cart> textboxScale = sprite->Scale();
  {
    auto msgGo = new GameObject{};
    auto text = new Text{*msgGo, ASSETS "/font/Call me maybe.ttf",
                         30,     Text::Blended,
                         ".",    colorFromHex("e23400")};
    msgGo->AddComponent(text);
    text->SetWrapWidth(sprite->SheetWidth());
    msgGo->box.x = MSG_BOX_X_OFFSET * textboxScale.x + textboxX;
    msgGo->box.y = MSG_BOX_Y_OFFSET * textboxScale.y + textboxY;
    msg = std::shared_ptr<GameObject>(msgGo);
  }

  {
    auto nameGo = new GameObject{};
    auto text = new Text{*nameGo, ASSETS "/font/Call me maybe.ttf",
                         70,      Text::Blended,
                         ".",     colorFromHex("e23400")};
    nameGo->AddComponent(text);
    text->SetWrapWidth(NAME_BOX_WIDTH);
    nameGo->box = Rect{NAME_BOX_X_OFFSET * textboxScale.x + textboxX,
                       NAME_BOX_Y_OFFSET * textboxScale.y + textboxY,
                       NAME_BOX_WIDTH, NAME_BOX_HEIGHT};
    name = std::shared_ptr<GameObject>(nameGo);
  }

  auto speakerGo = new GameObject{};
  auto speakerSprite = new Sprite{*speakerGo};
  speakerGo->AddComponent(speakerSprite);
  speakerSprite->SetScale(0.2);
  speakerGo->box.x = 100;
  speakerGo->box.y = 100;
  speaker = shared_ptr<GameObject>{speakerGo};
}

void Dialog::Start() {
  auto line = script[index++];
  ((Text *)msg->GetComponent(CType::Text))->SetText(line.content);
  ((Text *)name->GetComponent(CType::Text))->SetText(line.character);
  ((Sprite *)speaker->GetComponent(CType::Sprite))
      ->Open(ASSETS + std::string("/img/") + line.character + "_d.png");
}

void Dialog::Update(float) {
  auto &input = InputManager::Instance();

  if (input.KeyPress(NEXT_KEY)) {
    if (index >= (int)script.size()) {
      associated.RequestDelete();
      return;
    }

    auto line = script[index++];
    side = line.side;
    ((Text *)msg->GetComponent(CType::Text))->SetText(line.content);
    ((Text *)name->GetComponent(CType::Text))->SetText(line.character);
    ((Sprite *)speaker->GetComponent(CType::Sprite))
        ->Open(ASSETS + std::string("/img/") + line.character + "_d.png");
  }
}

void Dialog::Render(Vec2<Cart>) {
  speaker->Render(Vec2<Cart>{0, 0});
  textbox->Render(Vec2<Cart>{0, 0});
  msg->Render(Vec2<Cart>{0, 0});
  name->Render(Vec2<Cart>{0, 0});
  /*
    speaker->Render(camera);
    textbox->Render(camera);
    msg->Render(camera);
    name->Render(camera);
    speaker->Render(Vec2<Cart>{-300, -200});
    textbox->Render(Vec2<Cart>{-300, -400});
    msg->Render(Vec2<Cart>{-300, -500});
    name->Render(Vec2<Cart>{-320, -370});
  */
}

bool Dialog::Is(CType type) { return type == CType::Dialog; }
