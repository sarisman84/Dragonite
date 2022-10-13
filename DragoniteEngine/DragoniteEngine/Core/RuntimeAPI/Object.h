#pragma once
#include "Core/PollingStation.h"
#include "Core/CU/Transform.h"
#include "Component.h"
#include <vector>
#include <memory>
#include <string>
namespace Dragonite
{
	class Scene;
	class Object
	{
		friend class SceneBuilder;
	public:
		Object();
		Object(const char* aName, Scene* aCurrentScene);
		Object(const char* aName, const unsigned int aUUID);
		~Object();


		Object(const Object& aCpy);
		void operator=(const Object& aCpy);

		inline void Init(PollingStation* aStation) { myPollingStation = aStation; }


		template<typename TComponent>
		std::shared_ptr<TComponent> AddComponent();

		template<typename TComponent>
		std::shared_ptr<TComponent> GetComponent();


		inline const bool IsActive() const noexcept { return myActiveState; }
		inline void SetActive(const bool aNewValue) noexcept { myActiveState = aNewValue; }
		inline Transform& GetTransform() { return myTransform; }
		inline std::string& Name() { return myName; }
		inline std::vector<std::shared_ptr<Component>> Components() const noexcept { return myComponents; }

		void Awake();
		void Update(const float aDt);
		void ConstantUpdate();
		void OnDisable();

		inline unsigned int UUID() { return myUUID; }


		inline Scene* GetCurrentScene()  const noexcept { return myCurrentScene; }

	private:
		void GenerateID();


		unsigned int myUUID;
		std::string myName;
		bool myActiveState;
		std::vector<std::shared_ptr<Component>> myComponents;
		Transform myTransform;
		PollingStation* myPollingStation;
		Scene* myCurrentScene;

		Transform myInitialTransform;
	};

	template<typename TComponent>
	inline std::shared_ptr<TComponent> Object::AddComponent()
	{
		std::shared_ptr<Component> cmp = std::make_shared<TComponent>();
		cmp->myObject = this;
		cmp->myPollingStation = myPollingStation;
		cmp->OnCreate();
		myComponents.push_back(cmp);
		return std::dynamic_pointer_cast<TComponent>(cmp);
	}

	template<typename TComponent>
	inline std::shared_ptr<TComponent> Object::GetComponent()
	{
		auto it = std::find_if(myComponents.begin(), myComponents.end(), 
			[](std::shared_ptr<Component> aComponent)
			{
				auto val = std::dynamic_pointer_cast<TComponent>(aComponent);
				return val;
			});
		if (it == myComponents.end()) return nullptr;
		return std::dynamic_pointer_cast<TComponent>(*it);
	}

}

