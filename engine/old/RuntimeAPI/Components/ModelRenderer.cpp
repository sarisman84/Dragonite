#include "ModelRenderer.h"
#include "Core/Graphics/Models/Model.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/Graphics/GraphicsAPI.h"

#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/NEW/Scene.h"

#include "imgui/imgui.h"
#include "nlohmann/single_include/nlohmann/json.hpp"

#include <filesystem>

void Dragonite::ModelRenderer::Awake()
{

}

void Dragonite::ModelRenderer::Update(const float aDt)
{


}

void Dragonite::ModelRenderer::LateUpdate(const float aDt)
{
	if (!myModelInstance) return;

	RenderInstructions instruction;
	instruction.myTexture = myModelInstance->myTexture;
	instruction.myModelMatrix = myObject->myTransform.GetMatrix();
	instruction.myIndexBuffer = myModelInstance->myModel->myIndexBuffer;
	instruction.myIndexCount = myModelInstance->myModel->myIndexCount;
	instruction.myVertexBuffer = myModelInstance->myModel->myVertexBuffer;
	instruction.myShaderInstructionsID = myModelInstance->myShaderInstructionsID;
	instruction.myID = myObject->UUID();
	instruction.myProfileIndex = 0; //Use Perspective Camera

	myRenderInterface = myRenderInterface ? myRenderInterface : myObject->GetScene()->myPollingStation.Get<GraphicalInterface>();
	myRenderInterface->AddRenderInstructions(instruction);
}

void Dragonite::ModelRenderer::Start()
{
}

void* Dragonite::ModelRenderer::Serialize()
{
	using namespace nlohmann;

	static json data;

	data["model"] = myModelInstance->myModelName;
	data["material"] = myModelInstance->myMaterialName;
	data["texture"] = myModelInstance->myTextureName;



	return (void*)&data;
}

void Dragonite::ModelRenderer::Deserialize(void* someData)
{
	using namespace nlohmann;
	json data = *(json*)someData;

	myModelFactory = myModelFactory ? myModelFactory : myObject->GetScene()->myPollingStation.Get<ModelFactory>();

	std::filesystem::path p(data["texture"].get<std::wstring>());

	myModelInstance = myModelFactory->GetModel(data["model"].get<std::string>(), data["material"].get<std::string>(), p.wstring());

}

void Dragonite::ModelRenderer::OnInspectorGUI()
{
	std::filesystem::path p(myModelInstance->myTextureName);

	ImGui::Text("Model %s", myModelInstance->myModelName);
	ImGui::Text("Texture %s", p.string().c_str());
}

std::shared_ptr<Dragonite::Component> Dragonite::ModelRenderer::Clone()
{
	return std::make_shared<ModelRenderer>(*this);
}


