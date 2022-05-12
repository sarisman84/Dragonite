#include "ComponentManager.h"



EntityManager::EntityManager()
{

};

EntityID EntityManager::CreateEntity()
{
	EntityID anID = myActiveEntityAmm;
	myActiveEntityAmm++;
	return anID;
}

void EntityManager::DeleteEntity(EntityID anID)
{
	for (auto const& pair : myComponents)
	{
		auto& comp = pair.second;

		comp->OnEntityDestroyed(anID);
	}

	myActiveEntityAmm--;
}

