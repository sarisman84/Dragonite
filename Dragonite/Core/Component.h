#pragma once
#include "../EngineUtilities.h"
#include "System.h"
#include "Transform.h"

class GameObject;
class Component
{
public:
	Component() = default;
	friend class GameObject;
	inline Transform* GetTransform() { return myTransform; }
protected:

	virtual void OnAwake() = 0;
	virtual void OnUpdate(float aDeltaTime) = 0;
	GameObject* myGameObject = nullptr;
	Transform* myTransform = nullptr;
	Dragonite::System* mySystem = nullptr;
};