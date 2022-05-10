#include "ComponentManager.h"

ComponentManager::~ComponentManager()
{
	for (auto pair: mySystems)
	{
		if (pair.second)
			delete pair.second;

	}

	myComponents.clear();
}



void ComponentManager::UpdateComponents(float aTimeDelta)
{

	for (auto& pair : myComponents)
	{
		for (EntityID entity = 0; entity < pair.second.size(); ++entity)
		{
			mySystems[pair.first]->Update(entity, aTimeDelta);
		}

	}
}
