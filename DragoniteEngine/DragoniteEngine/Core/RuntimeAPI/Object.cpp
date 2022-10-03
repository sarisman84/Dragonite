#include "Object.h"

Dragonite::Object::Object() : myName("Empty Object"), myActiveState(true) {}

Dragonite::Object::Object(const char* aName) : myName(aName), myActiveState(true)
{
}

Dragonite::Object::~Object() = default;

Dragonite::Object::Object(const Object& aCpy)
{
	myActiveState = aCpy.myActiveState;
	myComponents = aCpy.myComponents;
	myName = aCpy.myName;
	myPollingStation = aCpy.myPollingStation;
	myTransform = aCpy.myTransform;

	for (auto& comp : myComponents)
	{
		comp->myObject = this;
		comp->myPollingStation = myPollingStation;
	}
}

void Dragonite::Object::operator=(const Object& aCpy)
{
	myActiveState = aCpy.myActiveState;
	myComponents = aCpy.myComponents;
	myName = aCpy.myName;
	myPollingStation = aCpy.myPollingStation;
	myTransform = aCpy.myTransform;

	for (auto& comp : myComponents)
	{
		comp->myObject = this;
		comp->myPollingStation = myPollingStation;
	}
}

void Dragonite::Object::Update(const float aDt)
{
	auto cpy = myComponents;
	for (auto& comp : cpy)
	{
		comp->Update(aDt);
	}
}
