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
	myAssetBrowser = nullptr;
}

void Dragonite::SceneEditor::OnWindowUpdate()
{
	if (ImGui::Button("Create New GameObject"))
	{
		InitializeNewObject();
	}

	auto& objs = myCurrentScene->SceneObjects();
	ImGui::Indent();
	for (size_t i = 0; i < objs.size(); i++)
	{
		bool selected = false;

		ImGui::Selectable(objs[i].Name().c_str(), &selected);
		if (selected)
		{
			myFocusedElement = i;
		}
	}
	ImGui::Unindent();





	if (!myViewport || !myPropertyEditor) return;

	myViewport->DisplayDebugInfo(myMouseInput);

	if (myMouseInput->GetButtonDown(MouseKey::Left) && myViewport->IsBeingFocused())
	{
		TryGetNewElement();
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
	auto el = myFocusedElement - 1;
	if (el < 0 || el > myCurrentScene->SceneObjects().size())
		return nullptr;
	return &myCurrentScene->SceneObjects()[el];
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

void Dragonite::SceneEditor::TryGetNewElement()
{
	int anID = -1;
	myViewport->TryGetObjectID(myMouseInput, anID);
	myFocusedElement = anID;
}





void Dragonite::SceneEditor::OnWindowInit()
{
	myAssetBrowser = dynamic_cast<AssetBrowser*>(GUIWindow::CreateEditorWindow(new AssetBrowser()));
	myViewport = dynamic_cast<Viewport*>(GUIWindow::CreateEditorWindow(new Viewport()));
	myPropertyEditor = dynamic_cast<PropertyEditor*>(GUIWindow::CreateEditorWindow(new PropertyEditor()));

	myAssetBrowser->RegisterSceneEditor(this);
	myViewport->RegisterSceneEditor(this);
	myPropertyEditor->RegisterSceneEditor(this);
	//GUIWindow::CreateEditorWindow(Inspector());
	//GUIWindow::CreateEditorWindow(Hierachy());

	myCurrentScene = myPollingStation->Get<Scene>();
	myModelFactory = myPollingStation->Get<ModelFactory>();
	myMouseInput = &myPollingStation->Get<InputManager>()->GetMouse();

}





