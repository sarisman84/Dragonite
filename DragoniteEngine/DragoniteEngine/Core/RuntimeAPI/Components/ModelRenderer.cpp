#include "ModelRenderer.h"
#include "Core/Graphics/Models/Model.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/Graphics/GraphicsAPI.h"

#include "Core/External/imgui/imgui.h"

void Dragonite::ModelRenderer::Awake()
{
	myModelFactory = myPollingStation->Get<ModelFactory>();
	myRenderInterface = myPollingStation->Get<GraphicalInterface>();
}

void Dragonite::ModelRenderer::Update(const float aDt)
{
	if (!myModelInstance || !myObject) return;

	RenderInstructions instruction;
	instruction.myTexture = myModelInstance->myTexture;
	instruction.myModelMatrix = myObject->GetTransform().GetMatrix();
	instruction.myIndexBuffer = myModelInstance->myModel->myIndexBuffer;
	instruction.myIndexCount = myModelInstance->myModel->myIndexCount;
	instruction.myVertexBuffer = myModelInstance->myModel->myVertexBuffer;
	instruction.myShaderInstructionsID = myModelInstance->myShaderInstructionsID;
	instruction.myID = myObject->UUID();

	myRenderInterface->AddRenderInstructions(instruction);

}

void Dragonite::ModelRenderer::OnInspectorGUI()
{

	ImGui::Text("Model: %s", myModelInstance->myModelName);
	ImGui::Text("Texture: %s", myModelInstance->myTextureName);


}

std::string Dragonite::ModelRenderer::GetName()
{
	return "Model Renderer";
}
