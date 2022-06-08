#pragma once
#include "Runtime.h"
#include "Components/ModelRenderer.h"
#include "Utilities/Algorithms/SparseSet.h"

using namespace Engine;

class Scene
{
public:
	Scene();
	void Awake();
	void Update(float aTimeDelta);
	Runtime* myRuntime;
private:
	Dragonite::SparseSet<const char*> mySet;
	GameObjectPtr myCamera;
	float myCounter;
	std::vector<GameObjectPtr> myObjectsToUpdate;
};