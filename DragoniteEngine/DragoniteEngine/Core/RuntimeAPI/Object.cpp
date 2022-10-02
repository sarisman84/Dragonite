#include "Object.h"

Dragonite::Object::Object() : myName("Empty Object"), myActiveState(true) {}

Dragonite::Object::Object(const char* aName) : myName(aName), myActiveState(true)
{
}

Dragonite::Object::~Object() = default;
