#include "Object.h"
#include "Scene.h"

Dragonite::Object::Object() : myName("Empty Object"), myActiveState(true) 
{
	myUUID = 0;
}

Dragonite::Object::Object(const char* aName, Scene* aCurrentScene) : myName(aName), myActiveState(true)
{
	myUUID = aCurrentScene->GetNextID();
}

Dragonite::Object::Object(const char* aName, const unsigned int aUUID) : myName(aName), myActiveState(true)
{
	myUUID = aUUID;
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
	myUUID = aCpy.myUUID;
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
	myUUID = aCpy.myUUID;
}

void Dragonite::Object::Awake()
{
	auto cpy = myComponents;
	for (auto& comp : cpy)
	{
		comp->Awake();
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

void Dragonite::Object::GenerateID()
{
	static unsigned int id = 1;
	myUUID = id++;
}
