#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Transform.h"



class Component;

namespace Dragonite
{
	class Runtime;
	class System;
}

class GameObject
{
public:

	GameObject();
	GameObject(std::string aName);


	template<class Comp>
	std::shared_ptr<Comp> AddComponent()
	{
		std::shared_ptr<Component> comp = std::make_shared<Comp>();
		comp->myGameObject = this;
		comp->myTransform = &myTransform;
		comp->mySystem = mySystem;
		comp->OnAwake();
		myComponents.push_back(comp);



		return std::dynamic_pointer_cast<Comp>(myComponents.back());
	}


	template<class Comp>
	std::shared_ptr<Comp> GetComponent()
	{
		for (size_t i = 0; i < myComponents.size(); i++)
		{
			std::shared_ptr<Comp> val = std::dynamic_pointer_cast<Comp>(myComponents[i]);
			if (val)
				return val;
		}

		return nullptr;
	}


	void SetActive(bool aValue);
	const bool IsActive();

	Transform myTransform;
	std::string myName;
	std::string myTag;
private:
	friend Dragonite::Runtime;
	void UpdateComponents(float aTimeDelta);

	Dragonite::System* mySystem = nullptr;
	bool myActiveState = true;
	std::vector<std::shared_ptr<Component>> myComponents = {};
};