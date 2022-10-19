#include "SceneEditor.h"
#include "Core/RuntimeAPI/NEW/Scene.h"
#include "Core/Graphics/Models/ModelFactory.h"

#include "Core/RuntimeAPI/NEW/Object.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/RuntimeAPI/Components/TestComponent.h"
#include "Core/RuntimeAPI/SceneManagement/SceneBuilder.h"

#include "Core/Utilities/Input.h"
#include "Core/Graphics/GraphicsAPI.h"
#include <string>
#include <fstream>

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
	myCurrentScene = myDragoniteGuiAPI->GetFocusedScene();


	if (!myCurrentScene) return;


	static bool hasExecutedCommand = false;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("context_menu");
	}

	if (ImGui::BeginPopup("context_menu"))
	{
		bool x = false;

		ImGui::MenuItem("Create", "", &x);

		if (ImGui::IsItemClicked())
		{
			ImGui::OpenPopup("create_context_menu");
		}

		if (ImGui::BeginPopup("create_context_menu"))
		{
			ImGui::MenuItem("Cube", "", &x);

			if (ImGui::IsItemClicked())
			{
				hasExecutedCommand = true;
				InitializeNewObject();

				ImGui::CloseCurrentPopup();
			}



			ImGui::MenuItem("Fancy Cube", "", &x);

			if (ImGui::IsItemClicked())
			{
				hasExecutedCommand = true;
				InitializeCustomObject();

				ImGui::CloseCurrentPopup();
			}


			ImGui::EndPopup();
		}

		if (hasExecutedCommand)
		{
			ImGui::CloseCurrentPopup();
			hasExecutedCommand = false;
		}



		ImGui::EndPopup();
	}






	//if (ImGui::Button("Create New GameObject"))
	//{
	//	InitializeNewObject();
	//}
	if (mySaveSceneFlag)
	{
		ImGui::OpenPopup("Save...");
		mySaveSceneFlag = false;
	}
	SaveSceneDefinition();




	auto& objs = myCurrentScene->SceneObjects();
	ImGui::Indent();
	for (size_t i = 0; i < objs.size(); i++)
	{
		bool selected = false;

		ImGui::Selectable(objs[i]->myName.c_str(), &selected);
		if (selected)
		{
			myFocusedElement = objs[i]->UUID();
		}
	}
	ImGui::Unindent();





	if (!myViewport || !myPropertyEditor) return;

	ImGui::Spacing();

	myViewport->DisplayDebugInfo(myMouseInput);

	if (myMouseInput->GetButtonDown(MouseKey::Left) && myViewport->IsBeingFocused() && myViewport->IsBeingHovered())
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
	myCurrentScene = myDragoniteGuiAPI->GetFocusedScene();
	if (!myCurrentScene) return nullptr;

	auto el = myFocusedElement;
	if (el < 0 || el >= myCurrentScene->SceneObjects().size())
		return nullptr;
	return myCurrentScene->SceneObjects()[el].get();
}

void Dragonite::SceneEditor::InitializeNewObject()
{
	if (!myCurrentScene) return;

	std::string name = "New GameObject ";
	name += "[" + std::to_string(myCurrentScene->SceneObjects().size()) + "]";
	Object* newObject = myCurrentScene->CreateObject(name);
	newObject->myTransform.myPosition = { 0,0, 1 };

	auto modelRenderer = newObject->AddComponent<ModelRenderer>();
	newObject->AddComponent<TestComponent>();

	modelRenderer->Model() = myModelFactory->GetModel(PrimitiveType::Cube, Material::defaultMaterial);



	myFocusedElement = myCurrentScene->SceneObjects().size();

}

void Dragonite::SceneEditor::InitializeCustomObject()
{
	std::string name = "New GameObject ";
	name += "[" + std::to_string(myCurrentScene->SceneObjects().size()) + "]";
	Object* newObject = myCurrentScene->CreateObject(name);
	newObject->myTransform.myPosition = { 0,0, 1 };

	auto modelRenderer = newObject->AddComponent<ModelRenderer>();

	modelRenderer->Model() = myModelFactory->GetModel(PrimitiveType::Cube, Material::defaultMaterial);


	myFocusedElement = myCurrentScene->SceneObjects().size();
}

void Dragonite::SceneEditor::TryGetNewElement()
{
	int anID = -1;
	myViewport->TryGetObjectID(myMouseInput, anID);
	myFocusedElement = anID;
}

void Dragonite::SceneEditor::SaveScene()
{
	mySaveSceneFlag = true;
}

bool Dragonite::SceneEditor::OpenFileExplorer(std::string& aPath, const _FILEOPENDIALOGOPTIONS anOptionsFlag, bool aSaveFile)
{
	std::string selectedFile;
	//  CREATE FILE OBJECT INSTANCE
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(f_SysHr))
		return false;


	PWSTR f_Path;
	if (aSaveFile)
	{
		IFileSaveDialog* f_FileSystem;
		f_SysHr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&f_FileSystem));
		if (FAILED(f_SysHr))
		{
			CoUninitialize();
			return false;
		}


		//  SHOW OPEN FILE DIALOG WINDOW
		f_SysHr = f_FileSystem->Show(NULL);
		if (FAILED(f_SysHr))
		{
			f_FileSystem->Release();
			CoUninitialize();
			return false;
		}

		//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
		IShellItem* f_Files;
		f_SysHr = f_FileSystem->GetResult(&f_Files);
		if (FAILED(f_SysHr))
		{
			f_FileSystem->Release();
			CoUninitialize();
			return false;
		}

		//  STORE AND CONVERT THE FILE NAME
		f_SysHr = f_Files->GetDisplayName(SIGDN_PARENTRELATIVE, &f_Path);
		if (FAILED(f_SysHr))
		{
			f_Files->Release();
			f_FileSystem->Release();
			CoUninitialize();
			return false;
		}

		f_Files->Release();
		f_FileSystem->Release();
	}
	else
	{
		// CREATE FileOpenDialog OBJECT
		IFileOpenDialog* f_FileSystem;
		f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
		if (FAILED(f_SysHr))
		{
			CoUninitialize();
			return false;
		}
		f_FileSystem->SetOptions(anOptionsFlag);


		//  SHOW OPEN FILE DIALOG WINDOW
		f_SysHr = f_FileSystem->Show(NULL);
		if (FAILED(f_SysHr))
		{
			f_FileSystem->Release();
			CoUninitialize();
			return false;
		}

		//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
		IShellItem* f_Files;
		f_SysHr = f_FileSystem->GetResult(&f_Files);
		if (FAILED(f_SysHr))
		{
			f_FileSystem->Release();
			CoUninitialize();
			return false;
		}
		//  STORE AND CONVERT THE FILE NAME
		f_SysHr = f_Files->GetDisplayName(anOptionsFlag == FOS_PICKFOLDERS ? SIGDN_FILESYSPATH : SIGDN_PARENTRELATIVE, &f_Path);
		if (FAILED(f_SysHr))
		{
			f_Files->Release();
			f_FileSystem->Release();
			CoUninitialize();
			return false;
		}

		//  SUCCESS, CLEAN UP
		f_Files->Release();
		f_FileSystem->Release();
	}


	//  FORMAT AND STORE THE FILE PATH
	std::wstring path(f_Path);
	std::string c(path.begin(), path.end());
	aPath = c;

	//  FORMAT STRING FOR EXECUTABLE NAME
	const size_t slash = aPath.find_last_of("/\\");
	selectedFile = aPath.substr(slash + 1);

	//  SUCCESS, CLEAN UP
	CoTaskMemFree(f_Path);
	//f_Files->Release();
	//f_FileSystem->Release();
	CoUninitialize();
	return true;
}

void Dragonite::SceneEditor::SaveSceneDefinition()
{
	static std::string name = myCurrentScene->myName;
	static std::string entry;
	if (ImGui::BeginPopupModal("Save..."))
	{

		if (ImGui::IsKeyDown(ImGuiKey_Escape))
		{
			ImGui::CloseCurrentPopup();
			name = myCurrentScene->myName;
		}

		ImGui::InputText("Name", &name, 0, DefaultStringResize);


		if (ImGui::Button("Select Directory"))
		{
			OpenFileExplorer(entry, FOS_PICKFOLDERS);
		}

		ImGui::SameLine();

		if (ImGui::Button("Save") || ImGui::IsKeyDown(ImGuiKey_Enter))
		{
			ImGui::CloseCurrentPopup();

			myCurrentScene->myName = name;
			entry += "\\" + name + ".json";
			myCurrentScene->Serialize(entry.c_str());
			name = myCurrentScene->myName;
		}


		ImGui::Text("Target Directory: %s", entry.c_str());

		ImGui::EndPopup();
	}
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

	myCurrentScene = myDragoniteGuiAPI->GetFocusedScene();
	myModelFactory = myPollingStation->Get<ModelFactory>();
	myMouseInput = &myPollingStation->Get<InputManager>()->GetMouse();





}





