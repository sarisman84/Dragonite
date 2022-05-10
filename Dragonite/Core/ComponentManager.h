#pragma once
#include <map>
#include <vector>
#include <typeinfo>
using EntityID = size_t;
using ComponentID = size_t;


//class Component
//{
//	friend class ComponentManager;
//public:
//	virtual void Awake(EntityID anID) = 0;
//	virtual void Update(EntityID anID, float aTimeDelta) = 0;
//private:
//	std::vector<EntityID> myEntities;
//};


class CompSystem
{
public:
	virtual void Awake(EntityID anID) = 0;
	virtual void Update(EntityID anID, float aTimeDelta) = 0;
private:

};


class ComponentManager
{
public:
	ComponentManager() = default;
	~ComponentManager();
	template<class Component>
	Component* GetComponent(EntityID anID);
	template<class Component>
	Component* AddComponent(EntityID anID);

	void UpdateComponents(float aTimeDelta);


private:
	std::map<ComponentID, std::vector<EntityID>> myComponents;
	std::map<ComponentID, CompSystem*> mySystems;
};

template<class Component>
inline Component* ComponentManager::GetComponent(EntityID anID)
{

	if (myData[anID])
		return static_cast<Component*>(myData[anID]);
	return nullptr;
}

template<class Component>
inline Component* ComponentManager::AddComponent(EntityID anID)
{
	ComponentID compID = typeid(Component).hash_code();

	if (std::find(myComponents[compID].begin(), myComponents[compID].end(), anID) == myComponents[compID].end())
		myComponents[compID].push_back(anID);

	return myComponents[compID];
}
