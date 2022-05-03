#include "Runtime.h"
#include  "System.h"
#include "Graphics/Rendering/Triangle.h"
#include "Graphics/Rendering/Quad.h"
#include "../CommonUtilities.h"

Engine::Runtime::Runtime(System* aCoreSystem)
{
	mySystem = aCoreSystem;
}

void Engine::Runtime::Awake()
{
	mySystem->GetGraphicsEngine()->CreateElement<Triangle>(Vector3<float>{0.f, 0.5f, 0.f}, Vector3<float>{0.5f, 0.5f, 0.5f});

	mySystem->GetGraphicsEngine()->CreateElement<Quad>(Vector3<float>{0.f, 0.0f, 0.f}, Vector3<float>{0.25f, 0.5f, 0.5f});
}

void Engine::Runtime::Update()
{
}

