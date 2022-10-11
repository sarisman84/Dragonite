#include "SceneEditor.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/Graphics/Models/ModelFactory.h"

#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/RuntimeAPI/SceneManagement/SceneBuilder.h"

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
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		ImGui::OpenPopup("Save...");

	}
	SaveSceneDefinition();




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
	if (el < 0 || el >= myCurrentScene->SceneObjects().size())
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

	myFocusedElement = myCurrentScene->SceneObjects().size();

}

void Dragonite::SceneEditor::TryGetNewElement()
{
	int anID = -1;
	myViewport->TryGetObjectID(myMouseInput, anID);
	myFocusedElement = anID;
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
	static std::string name = "New Scene";
	static std::string entry;
	if (ImGui::BeginPopupModal("Save..."))
	{


		ImGui::InputText("Name", &name, 0, DefaultStringResize);


		if (ImGui::Button("Select Directory"))
		{
			OpenFileExplorer(entry, FOS_PICKFOLDERS);
		}


		if (ImGui::IsKeyDown(ImGuiKey_Escape))
		{
			ImGui::CloseCurrentPopup();
			name = "New Scene";
		}

		if (ImGui::Button("Save") || ImGui::IsKeyDown(ImGuiKey_Enter))
		{
			ImGui::CloseCurrentPopup();
			name = "New Scene";
			myCurrentScene->Name() = name;
			entry += "\\" + name + ".json";
			SceneBuilder::SaveScene(entry.c_str(), *myCurrentScene);
		}

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

	myCurrentScene = myPollingStation->Get<Scene>();
	myModelFactory = myPollingStation->Get<ModelFactory>();
	myMouseInput = &myPollingStation->Get<InputManager>()->GetMouse();

}





