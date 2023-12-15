#include "component/Coin.h"

#include "Camera.h"
#include "component/CoinCounter.h"
#include "InputManager.h"
#include "util.h"

#define MODULE "Coin"

Coin::Coin(GameObject& go): Component(go) {
}

void Coin::Update(float dt) {
	if (moving) {
		Vec2<Cart> pos = associated.box.Center();
		auto camera = Game::Instance().GetState().GetCamera();
		target = camera.Pos() + Vec2<Cart>{SCREEN_WIDTH-113, 62};
		speed = (target - pos).normalize() * accel;
		accel += 10;
		Vec2<Cart> nPos = pos + speed * dt;
		if (nPos.dist(target) < 5.0) {
			nPos = target;
			moving = false;
			CoinCounter::coinCounter->AddCoin();
			associated.RequestDelete();
		}
		associated.box.SetCenter(nPos);
	}
}

void Coin::NotifyCollisionEnter(GameObject& other) {
    if (!collected && other.tags.test(tag::Player)) {
		collected = true;
		moving = true;
		associated.renderLayer = 200;
	}
}
