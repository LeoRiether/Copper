#pragma once

#include "Component.h"
#include "component/Text.h"
#include "CType.h"
#include <string>

class Tooltip : public Component {
   public:
	Tooltip(GameObject& go);
	static Tooltip* tooltip;

	void Update(float dt);

	inline CType Key() const { return CType::Tooltip; }

	void SetText(std::string str);
	void FadeIn();
	void FadeOut();

   private:
    constexpr static float fadingTime{0.5};
  	float fadingIn{0}, fadingOut{0};
	bool invis{true};

};

