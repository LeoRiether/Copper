#include "Component.h"

#define MODULE "Component"

Component::Component(GameObject& associated) : associated(associated) {}

Component::~Component() {}
