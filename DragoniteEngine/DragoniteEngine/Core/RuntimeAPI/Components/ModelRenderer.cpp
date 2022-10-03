#include "ModelRenderer.h"
#include "Core/Graphics/Models/Model.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/Graphics/GraphicsAPI.h"

#include "Core/External/imgui/imgui.h"

void Dragonite::ModelRenderer::Awake()
{
	myModelFactory = myPollingStation->Get<ModelFactory>();
	myRenderInterface = myPollingStation->Get<RenderInterface>();
}

void Dragonite::ModelRenderer::Update(const float aDt)
{
	if (!myModel) return;
	myModel->myTransform = myObject->GetTransform();
	myRenderInterface->DrawElement(myModel);

}

void Dragonite::ModelRenderer::OnInspectorGUI()
{

	ImGui::Text("Model: %s", myModel->myModelName);
	ImGui::Text("Texture: %s", myModel->myTextureName);


}
