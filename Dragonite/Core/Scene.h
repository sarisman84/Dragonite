#pragma once
#include "Runtime.h"
#include "Components/ModelRenderer.h"

using namespace Engine;

class Scene
{
public:
	void Awake();
	void Update(float aTimeDelta);
	Runtime* myRuntime;
private:
	GameObjectPtr myCamera;
	GameObjectPtr myCube;
	float myCounter;
	std::shared_ptr<ModelRenderer> myCubeRenderer;
};