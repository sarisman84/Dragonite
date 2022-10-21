#include "Object.h"
#include "Core/RuntimeAPI/Component.h"
Dragonite::Object::Object(std::string aName)
{
	myName = aName;

}

void Dragonite::Object::Start()
{
	for (size_t i = 0; i < myComponents.size(); i++)
	{
		myComponents[i]->Awake();
		myComponents[i]->Start();
	}
}

void Dragonite::Object::Update(const float aDt)
{
	for (size_t i = 0; i < myComponents.size(); i++)
	{
		myComponents[i]->Update(aDt);
	}
}

void Dragonite::Object::LateUpdate(const float aDt)
{
	for (size_t i = 0; i < myComponents.size(); i++)
	{
		myComponents[i]->LateUpdate(aDt);
	}
}

void Dragonite::Object::AddComponent(const std::shared_ptr<Component>& aCpy)
{
	myComponents.push_back(aCpy->Clone());
}
