#include "component/Exploder.h"

#include "component/Bullet.h"
#include "component/Collider.h"
#include "component/OverheadHpBar.h"
#include "component/Sprite.h"
#include "Game.h"
#include "Prefabs.h"
#include "util.h"

#define MODULE "Exploder"

Exploder::Exploder(GameObject& go): Component(go) {}

void Exploder::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    bool meleeHit = other.tags.test(tag::PlayerHitbox);
    if (bullet || meleeHit) {
        auto bar =
            (OverheadHpBar*)associated.GetComponent(CType::OverheadHpBar);
        if (bar) {
            bar->SetHp(bar->Hp() - 25);
        }

        if (bar && bar->Hp() <= 0) {
            Die();
            other.RequestDelete();
            return;
        }
		
        // Trauma
        if (meleeHit) {
            Game::Instance().AddTrauma(0.3);
            Game::Instance().Slowdown(0.03, 0.2);
        }

        // Flash
        for (auto& sprite : associated.GetAllComponents(CType::Sprite)) {
            ((Sprite*)sprite.get())->WithFlash(0.08);
        }

        other.RequestDelete();
    }
}

Exploder* Exploder::WithHp(int hp) {
    auto bar = (OverheadHpBar*)associated.GetComponent(CType::OverheadHpBar);
    if (!bar) fail("WithHp called, but no associated OverheadHpBar");
    bar->SetHp(hp);
    return this;
}

void Exploder::Die() {
    auto center = associated.box.Center();
	center.y -= 70;
	auto explosion = MakeExplosion3(Vec2<Cart>{3, 3})->WithCenterAt(center);
	//TODO make collider hit once
	auto s = (Sprite*)explosion->GetComponent(CType::Sprite);
	explosion->box.w = s->SheetWidth();
	explosion->box.h = s->SheetHeight();
	explosion->AddComponent(new Collider{*explosion});
	explosion->tags.set(tag::Bullet);
	explosion->tags.set(tag::PlayerHitbox);
	explosion->tags.set(tag::EnemyHitbox);
	explosion->tags.set(tag::Explosion);
    associated.RequestAdd(explosion);
    associated.RequestDelete();
	MakeOneOffAudio(ASSETS "/audio/BigExplosion.wav", 30);
}
