#pragma once
#include <string>
#include "Core/CU/Transform.h"
#include "Core/RuntimeAPI/Component.h"

#include <vector>
#include <memory>
#include <typeinfo>

namespace Dragonite
{
	template<typename T>
	using Comp = std::shared_ptr<T>;

	class Scene;
	class Object
	{
		friend Scene;
	public:
		Object(std::string aName = "New GameObject");

		void Start();
		void Update(const float aDt);
		void LateUpdate(const float aDt);


		template<typename T>
		Comp<T> AddComponent();

		template<typename T>
		Comp<T> GetComponent();


		Transform myTransform;
		std::string myName;


		inline unsigned int UUID() { return myUUID; }
		inline Scene* GetScene() { return myCurrentScene; }
		inline std::vector<std::shared_ptr<Component>>& Components() { return myComponents; }

	private:
		unsigned int myUUID;
		Scene* myCurrentScene;
		std::vector<std::shared_ptr<Component>> myComponents;
	};





	template<typename T>
	inline Comp<T> Object::AddComponent()
	{
		Comp<Component> comp = std::make_shared<T>();

		comp->myName = typeid(T).name();
		comp->myObject = this;

		myComponents.push_back(comp);

		return std::dynamic_pointer_cast<T>(comp);
	}
	template<typename T>
	inline Comp<T> Object::GetComponent()
	{
		auto it = std::find_if(myComponents.begin(), myComponents.end(), [](Comp<Component> aComponent)
			{
				return std::dynamic_pointer_cast<T>(aComponent);
			});

		if (it != myComponents.end())
			return std::dynamic_pointer_cast<T>(*it);
		else
			return nullptr;
	}
}