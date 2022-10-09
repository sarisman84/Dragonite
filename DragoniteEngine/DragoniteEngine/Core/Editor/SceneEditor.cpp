#include "SceneEditor.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/Utilities/Input.h"
#include "Core/Graphics/GraphicsAPI.h"
#include <string>

#include "Core/External/imgui/misc/cpp/imgui_stdlib.h"
#include "Core/Graphics/RenderTargets/RenderFactory.h"
#include "Core/Utilities/Input.h"
#include "Core/ImGui/DragoniteGui.h"

#include "SceneEditor/Viewport.h"
#include "SceneEditor/PropertyEditor.h"
#include "AssetBrowser.h"




Dragonite::SceneEditor::SceneEditor() : GUIWindow("Scene Editor")
{

}

Dragonite::SceneEditor::~SceneEditor()
{
	myViewport = nullptr;
	myPropertyEditor = nullptr;
}

void Dragonite::SceneEditor::OnWindowUpdate()
{


	//if (!myCurrentScene) return;

	//if (ImGui::Button("Add Object"))
	//{
	//	//myCurrentScene->SceneObjects().push_back(myModelFactory->GetModel(PrimitiveType::Cube, Material::defaultMaterial));

	//	Object newObject = Object("New GameObject");
	//	newObject.Init(myPollingStation);
	//	newObject.GetTransform().myPosition = { 0,0, 1 };

	//	auto modelRenderer = newObject.AddComponent<ModelRenderer>();

	//	modelRenderer->Model() = myModelFactory->GetModel(PrimitiveType::Cube, Material::defaultMaterial);


	//	myCurrentScene->SceneObjects().push_back(newObject);
	//	mySelectedElements.push_back(false);


	//	FocusElement(mySelectedElements.size() - 1);
	//}



	//{
	//	std::string name = "Camera";
	//	name += std::string("##") + std::to_string(0);
	//	static bool val;
	//	ImGui::Selectable(name.c_str(), &val);
	//	if (val)
	//		FocusElement(0);


	//	int index = 0;

	//	auto& sceneObjs = myCurrentScene->SceneObjects();
	//	for (size_t i = 0; i < sceneObjs.size(); i++)
	//	{
	//		val = false;
	//		auto& object = sceneObjs[i];
	//		std::string name = sceneObjs[i].Name();
	//		name += std::string("##") + std::to_string(i);
	//		ImGui::Selectable(name.c_str(), &val);
	//		if (val)
	//			FocusElement(i + 1);
	//		val = false;

	//	}
	//}
	//static int lastFoundElement = 0;

	//auto s = true;
	//if (!s)
	//	if (myMouseInput->GetButtonDown(MouseKey::Left))
	//	{
	//		int element = 0;
	//		if (myRenderID.TryGetElement(myMouseInput, element))
	//		{
	//			FocusElement(element);
	//			lastFoundElement = element;
	//		}
	//		else
	//		{
	//			ResetFocus();
	//			lastFoundElement = 0;
	//		}
	//	}




	//ImGui::Begin("Render ID Debugger");
	//ImGui::Text("Hover Element: %i", lastFoundElement);
	//ImGui::End();


	//ImGui::Begin("Property");
	//myPropertyFocus = ImGui::IsWindowHovered() ||
	//	ImGui::IsWindowFocused() ||
	//	ImGui::IsAnyItemFocused();

	//if (IsInspectingFocusedElement())
	//{
	//	if (myFocusedElement == 0)
	//		InspectCamera(myCurrentScene->GetCamera());
	//	else
	//		InspectFocusedElement(myCurrentScene);
	//}
	//ImGui::End();





	ImGui::Text("Selected Element: %i", myFocusedElement);
	if (ImGui::Button("Create New GameObject"))
	{
		InitializeNewObject();
	}

	auto& objs = myCurrentScene->SceneObjects();
	for (size_t i = 0; i < objs.size(); i++)
	{
		bool selected = false;

		ImGui::Selectable(objs[i].Name().c_str(), &selected);
		if (selected) {
			myFocusedElement = i;
		}
	}




	if (!myViewport || !myPropertyEditor) return;

	myViewport->DisplayMouseCoordinateInViewport(myMouseInput);

	if (myMouseInput->GetButtonDown(MouseKey::Left) && myViewport->IsBeingFocused()) {
		int anID = -1;
		myViewport->TryGetObjectID(myMouseInput, anID);
		myFocusedElement = anID;
	}







}

void Dragonite::SceneEditor::OnEnable()
{
}

void Dragonite::SceneEditor::OnDisable()
{
}


Dragonite::Object* Dragonite::SceneEditor::GetInspectedObject()
{
	if (myFocusedElement < 0 || myFocusedElement > myCurrentScene->SceneObjects().size())
		return nullptr;
	return &myCurrentScene->SceneObjects()[myFocusedElement];
}

void Dragonite::SceneEditor::InitializeNewObject()
{
	if (!myCurrentScene) return;

	std::string name = "New GameObject ";
	name += "[" + std::to_string(myCurrentScene->SceneObjects().size()) + "]";
	Object newObject = Object(name.c_str());
	newObject.Init(myPollingStation);
	newObject.GetTransform().myPosition = { 0,0, 1 };

	auto modelRenderer = newObject.AddComponent<ModelRenderer>();

	modelRenderer->Model() = myModelFactory->GetModel(PrimitiveType::Cube, Material::defaultMaterial);


	myCurrentScene->SceneObjects().push_back(newObject);

	myFocusedElement = myCurrentScene->SceneObjects().size() - 1;

}





void Dragonite::SceneEditor::OnWindowInit()
{

	myViewport = dynamic_cast<Viewport*>(GUIWindow::CreateEditorWindow(new Viewport()));
	myPropertyEditor = dynamic_cast<PropertyEditor*>(GUIWindow::CreateEditorWindow(new PropertyEditor()));
	auto assetBrowser = dynamic_cast<AssetBrowser*>(GUIWindow::CreateEditorWindow(new AssetBrowser()));
	assetBrowser->RegisterSceneEditor(this);
	myViewport->RegisterSceneEditor(this);
	myPropertyEditor->RegisterSceneEditor(this);
	//GUIWindow::CreateEditorWindow(Inspector());
	//GUIWindow::CreateEditorWindow(Hierachy());

	myCurrentScene = myPollingStation->Get<Scene>();
	myModelFactory = myPollingStation->Get<ModelFactory>();
	myMouseInput = &myPollingStation->Get<InputManager>()->GetMouse();

}





