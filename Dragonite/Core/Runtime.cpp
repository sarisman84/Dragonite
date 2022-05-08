#include "Runtime.h"
#include  "System.h"
#include "Utilities/UtilityFunctions.h"

Engine::Runtime::Runtime(System* aCoreSystem)
{
	mySystem = aCoreSystem;
}

void Engine::Runtime::Awake()
{

	Object triangle = mySystem->GetGraphicsEngine()->CreateElement(Primitive::Triangle);
	Object quad = mySystem->GetGraphicsEngine()->CreateElement(Primitive::Quad);
	someShape = mySystem->GetGraphicsEngine()->CreateElement(Primitive::Circle);
	someOtherShape = mySystem->GetGraphicsEngine()->CreateElement(Primitive::Circle);


	someShape->SetSize({ 0.25f, 0.25f, 0.25f });
	someOtherShape->SetSize({ 0.25f, 0.25f, 0.25f });
	triangle->SetSize({ 2.f, 1.f, 1.0f });
	quad->SetSize({ 0.5f, 0.5f, 0.5f });
	quad->SetPixelShader("Shaders/SinWave_PS.cso");
	someShape->SetPosition({ 0.5f, 0.f, 0.f });
	someOtherShape->SetPixelShader("Shaders/SinWave_PS.cso");
	someShape->SetPixelShader("Shaders/SinWave_PS.cso");
}

void Engine::Runtime::Update()
{
	someCount++;
	Math::Vector3f pos = { sin(someCount * 0.01f), 0.f, 0.f };
	pos.x = Utilities::Clamp(-0.3f, 0.3f, pos.x);
	someShape->SetPosition(pos);

	pos = { -sin(someCount * 0.01f), 0.f, 0.f };
	pos.x = Utilities::Clamp(-0.3f, 0.3f, pos.x);


	someOtherShape->SetPosition(pos);
}

