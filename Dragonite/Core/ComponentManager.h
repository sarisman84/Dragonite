#pragma once
#include <map>
#include <vector>
#include <typeinfo>
#include <cassert>
#include <memory>


using Signature = std::vector<bool>;
using EntityID = size_t;
using ComponentID = size_t;


class BaseCompObj
{
public:
	virtual ~BaseCompObj() = default;
	virtual void OnEntityDestroyed(EntityID anID) = 0;
};

template<class Component>
class CompObj : public BaseCompObj
{
public:
	void InitData(EntityID anEntity, Component anComponent)
	{
		myEntityToIndexMap[anEntity] = myData.size();
		myIndexToEntityMap[myData.size()] = anEntity;
		myData.push_back(anComponent);
	}

	void ClearData(EntityID anEntity)
	{
		EntityID removedEntity = size_t();
		for (auto& pair : myEntityToIndexMap)
		{
			if (pair.first == anEntity)
			{
				removedEntity = pair.second;
				break;
			}

		}
		if (removedEntity == size_t()) return;
		size_t lastElement = myData.size() - 1;
		myData[removedEntity] = myData[lastElement];

		EntityID lastEntity = myIndexToEntityMap[lastElement];
		myEntityToIndexMap[lastEntity] = lastElement;
		myIndexToEntityMap[removedEntity] = lastEntity;


		myEntityToIndexMap.erase(anEntity);
		myIndexToEntityMap.erase(lastElement);

		myData.pop_back();
	}

	Component& GetData(EntityID anID)
	{
		assert(myEntityToIndexMap.find(anID) != myEntityToIndexMap.end());
		return myData[myEntityToIndexMap[anID]];
	}

	void OnEntityDestroyed(EntityID anID) override
	{
		if (myEntityToIndexMap.find(anID) != myEntityToIndexMap.end())
		{
			ClearData(anID);
		}
	}

private:
	std::vector<Component> myData;
	std::map<EntityID, size_t> myEntityToIndexMap;
	std::map<size_t, EntityID> myIndexToEntityMap;
};

namespace Engine
{
	class System;
}

class EntityManager
{
public:
	EntityManager();
	inline void FetchSystem(Engine::System* aSystem) { mySystem = aSystem; }
	inline Engine::System* GetSystem() { return mySystem; }

	EntityID CreateEntity();
	void DeleteEntity(EntityID anID);


	template<class Component, typename... Args>
	Component& AddComponent(EntityID anID, Args&&... someArgs);

	template<class Component>
	Component& GetComponent(EntityID anID);

	template<class Component>
	void RemoveComponent(EntityID anID);


	template<class Component>
	void RegisterUpdateCall(void(*aCallback)(EntityID, Component&, EntityManager*));



	template<class Component>
	void UpdateComponent();
private:

	template<class Component>
	void RegisterComponent();


	template<class Component>
	std::shared_ptr<CompObj<Component>> GetComponentArray()
	{
		const char* compName = typeid(Component).name();

		assert(myComponents.find(compName) != myComponents.end());

		return std::static_pointer_cast<CompObj<Component>>(myComponents[compName]);
	}

	std::map < const char*, void*> myUpdateCallbacks;
	std::map <const char*, std::shared_ptr<BaseCompObj>> myComponents;
	EntityID myActiveEntityAmm;
	Engine::System* mySystem;
};

template<class Component, typename... Args>
inline Component& EntityManager::AddComponent(EntityID anID, Args&&... someArgs)
{
	const char* compName = typeid(Component).name();
	// // O: insert return statement here
	if (myComponents.find(compName) == myComponents.end())
	{
		RegisterComponent<Component>();
	}


	GetComponentArray<Component>()->InitData(anID, Component{ someArgs... });

	return GetComponent<Component>(anID);
}

template<class Component>
inline Component& EntityManager::GetComponent(EntityID anID)
{
	return GetComponentArray<Component>()->GetData(anID);
	// // O: insert return statement here
}

template<class Component>
inline void EntityManager::RemoveComponent(EntityID anID)
{
	GetComponentArray<Component>()->ClearData(anID);
}

template<class Component>
inline void EntityManager::RegisterUpdateCall(void(*aCallback)(EntityID, Component&, EntityManager*))
{
	const char* compName = typeid(Component).name();
	myUpdateCallbacks[compName] = static_cast<void*>(aCallback);
}

template<class Component>
inline void EntityManager::UpdateComponent()
{
	const char* compName = typeid(Component).name();
	for (EntityID i = 0; i < myActiveEntityAmm; i++)
	{
		auto callback = static_cast<void(*)(EntityID, Component&, EntityManager*)>(myUpdateCallbacks[compName]);
		if (callback)
			callback(i, GetComponentArray<Component>()->GetData(i), this);
	}


}

template<class Component>
inline void EntityManager::RegisterComponent()
{
	const char* compName = typeid(Component).name();
	myComponents.insert({ compName, std::make_shared<CompObj<Component>>() });
}





