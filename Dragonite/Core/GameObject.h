#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Transform.h"



class Component;

namespace Engine
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
		std::shared_ptr<Comp> comp = std::make_shared<Comp>();
		comp->myGameObject = this;
		comp->myTransform = &myTransform;
		comp->mySystem = mySystem;

		myComponents.push_back(comp);



		return comp;
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
	friend Engine::Runtime;
	void UpdateComponents(float aTimeDelta);

	Engine::System* mySystem = nullptr;
	bool myActiveState = true;
	std::vector<std::shared_ptr<Component>> myComponents = {};
};