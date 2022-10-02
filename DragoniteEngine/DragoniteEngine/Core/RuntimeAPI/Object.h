#pragma once
#include "Core/PollingStation.h"
#include "Core/CU/Transform.h"
#include "Component.h"
#include <vector>
#include <memory>
namespace Dragonite
{
	class Object
	{
	public:
		Object();
		Object(const char* aName);
		~Object();


		inline void Init(PollingStation* aStation) { myPollingStation = aStation; }


		template<typename TComponent>
		std::shared_ptr<TComponent> AddComponent();

		template<typename TComponent>
		std::shared_ptr<TComponent> GetComponent();


		inline const bool IsActive() const noexcept { return myActiveState; }
		inline void SetActive(const bool aNewValue) noexcept { myActiveState = aNewValue; }
		inline Transform& GetTransform() { return myTransform; }
	private:
		const char* myName;
		bool myActiveState;
		std::vector<std::shared_ptr<Component>> myComponents;
		Transform myTransform;
		PollingStation* myPollingStation;
	};

	template<typename TComponent>
	inline std::shared_ptr<TComponent> Object::AddComponent()
	{
		std::shared_ptr<Component> cmp = std::make_shared<TComponent>();
		cmp->Awake();
		myComponents.push_back(cmp);
		return std::dynamic_pointer_cast<TComponent>(cmp);
	}

	template<typename TComponent>
	inline std::shared_ptr<TComponent> Object::GetComponent()
	{
		auto it = std::find_if(myComponents.begin(), myComponents.end(), [](Component* aComponent) { return typeid(aComponent) == typeid(TComponent); });
		if (it == myComponents.end()) return nullptr;
		return *it;
	}

}

