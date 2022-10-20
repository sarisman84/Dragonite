#include "SpriteRenderer.h"
#include "Core/External/imgui/imgui.h"
#include "Core/External/nlohmann/json.hpp"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/RuntimeAPI/NEW/Scene.h"

#include "Core/Graphics/Models/ModelFactory.h"

#include "Core/CU/Math/Matrix4x4.hpp"

void Dragonite::SpriteRenderer::Awake()
{
}

void Dragonite::SpriteRenderer::Start()
{
}

void Dragonite::SpriteRenderer::Update(const float aDt)
{
}

void Dragonite::SpriteRenderer::LateUpdate(const float aDt)
{
	if (!mySpriteInstance) return;

	RenderInstructions instruction;
	instruction.myTexture = mySpriteInstance->myTexture;
	instruction.myModelMatrix = GetLocal2DMatrix();
	instruction.myIndexBuffer = mySpriteInstance->myModel->myIndexBuffer;
	instruction.myIndexCount = mySpriteInstance->myModel->myIndexCount;
	instruction.myVertexBuffer = mySpriteInstance->myModel->myVertexBuffer;
	instruction.myShaderInstructionsID = mySpriteInstance->myShaderInstructionsID;
	instruction.myID = myObject->UUID();
	instruction.myProfileIndex = 1; //Use Orthographic Camera

	myInterface = myInterface ? myInterface : myObject->GetScene()->myPollingStation.Get<GraphicalInterface>();
	myInterface->AddRenderInstructions(instruction);
}

void* Dragonite::SpriteRenderer::Serialize()
{
	using namespace nlohmann;

	static json data;

	data["shape"] = mySpriteInstance->myModelName;
	data["material"] = mySpriteInstance->myMaterialName;
	data["sprite"] = mySpriteInstance->myTextureName;



	return (void*)&data;
}

void Dragonite::SpriteRenderer::Deserialize(void* someData)
{
	using namespace nlohmann;
	json data = *(json*)someData;

	myModelFactory = myModelFactory ? myModelFactory : myObject->GetScene()->myPollingStation.Get<ModelFactory>();

	std::filesystem::path p(data["sprite"].get<std::u8string>());

	mySpriteInstance = myModelFactory->GetModel(data["shape"].get<std::string>(), data["material"].get<std::string>(), p.wstring());
}

void Dragonite::SpriteRenderer::OnInspectorGUI()
{
	std::filesystem::path p(mySpriteInstance->myTextureName);

	ImGui::Text("Sprite Type: %s", mySpriteInstance->myModelName);
	ImGui::Text("Sprite: %s", p.string().c_str());
}

Dragonite::Matrix4x4f Dragonite::SpriteRenderer::GetLocal2DMatrix()
{
	constexpr float pieVal = 3.141592653589793238f;

	Matrix4x4f result;

	float radRot = (myObject->myTransform.myRotation.y) * (pieVal / 180.0f);

	Vector3f lPivot = Vector3f(-0.5f, -0.5f, 0.0f);

	Matrix4x4f size = Matrix4x4f::CreateScaleMatrix(myObject->myTransform.myScale);

	Matrix4x4f rot = Matrix4x4f::CreateRotationAroundZ(radRot);

	Matrix4x4f m = size * rot;
	Vector4f p = (lPivot * m) + Vector4f(myObject->myTransform.myPosition, 1.0f);

	result = Matrix4x4f();

	result(1, 1) = m(1, 1);
	result(1, 2) = m(1, 2);
	result(2, 1) = m(2, 1);
	result(2, 2) = m(2, 2);
	result(4, 1) = p.x;
	result(4, 2) = p.y;

	return result;
}
