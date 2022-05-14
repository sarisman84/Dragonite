#include "GameObject.h"
#include <functional>
#include "Component.h"
#include "Utilities/Extensions.h"

GameObject::GameObject()
{
	myComponents = std::vector<std::shared_ptr<Component>>();

}

GameObject::GameObject(std::string aName)
{
	myName = aName;
	myComponents = std::vector<std::shared_ptr<Component>>();
}

void GameObject::SetActive(bool aValue)
{
	myActiveState = aValue;
}

const bool GameObject::IsActive()
{
	return myActiveState;
}

void GameObject::UpdateComponents(float aTimeDelta)
{
	for (size_t i = 0; i < myComponents.size(); i++)
	{
		myComponents[i]->OnUpdate(aTimeDelta);
	}
}
