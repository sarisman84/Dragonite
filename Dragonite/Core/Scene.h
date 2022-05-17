#pragma once
#include "Runtime.h"

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
};