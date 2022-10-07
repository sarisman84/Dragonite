#include "AssetBrowser.h"
#include "Commands/MenuCommand.h"

#include "Core/Graphics/Textures/TextureFactory.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/ImGui/DragoniteGui.h"
#include "Core/Editor/SceneEditor.h"

#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"

#include "Core/Graphics/Models/Model.h"

Dragonite::AssetBrowser::AssetBrowser() : GUIWindow("Project")
{
}

void Dragonite::AssetBrowser::OnWindowInit()
{
	myAssetBrowserDirectory.assign("resources");

	myCreateFolderCommand.reset(new FolderCommand(FolderCommand::Type::Create));
	myDeleteFolderCommand.reset(new FolderCommand(FolderCommand::Type::Remove));

	myCreateFolderWindow.reset(new ModalWindow("Create Folder", [this](void* someData) mutable
		{
			static char input[200];

			std::string path = ((Directory*)someData)->path().string();

			ImGui::Text("Set Folder Name (Max %i Characters)", sizeof(input));
			ImGui::InputText("##SetFolderName", input, sizeof(input));

			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				std::string targetPath = path + "/" + std::string(input);
				memset(input, 0, sizeof(input));
				myCreateFolderCommand->Execute(&targetPath);
				myCreateFolderWindow->Close();
			}
		}));


	myFolderMenu.reset(new PopupMenu());
	myFolderMenu->AddMenuElement("Create Folder", [this](void* someData)
		{
			mySelectedFolderDirToEdit = *(Directory*)someData;
			myCreateFolderWindow->Open();
		});
	myFolderMenu->AddMenuElement("Delete Folder", [this](void* someData)
		{
			auto path = ((Directory*)someData)->path().string();
			myDeleteFolderCommand->Execute(&path);
		});

	myDragCommand.reset(new DragCommand());

	myScene = myPollingStation->Get<Scene>();
	myTextureFactory = myPollingStation->Get<TextureFactory>();



	myFolderIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/open-folder.dds");
	myUnknownArtAssetIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/large-paint-brush.dds");
	myJsonIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/paper-clip.dds");


	mySelectedFolderDir = myAssetBrowserDirectory;
	mySceneEditor = (SceneEditor*)myDragoniteGuiAPI->GetWindow("Hierachy").get();
	/*myCurrentScene = myPollingStation->Get<Scene>();
	myModelFactory = myPollingStation->Get<ModelFactory>();*/


	/*mySceneEditor->OnFocusedElement() = [this](Object& anElementToFocus)
	{
		if (mySelectedAsset != Directory() &&
			mySelectedAsset.path().filename().extension() == ".png" ||
			mySelectedAsset.path().filename().extension() == ".dds")
		{
			auto renderer = anElementToFocus.GetComponent<ModelRenderer>();
			renderer->Model()->myTexture = myTextureFactory->LoadTexture(mySelectedAsset.path().wstring().c_str());

			mySelectedAsset = Directory();
		}
	};*/
}

void Dragonite::AssetBrowser::OnWindowRender()
{
	auto extension = mySelectedAsset.path().filename().extension();
	bool isDDS = extension == ".dds";
	bool isShader = extension == ".cso";
	bool isDirectory = mySelectedAsset.is_directory();
	bool isImage = isDDS || extension == ".png";


	//if (ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.5f) && isImage && mySceneEditor->IsInspectingFocusedElement() && mySelectedAsset != Directory())
	//{
	//	auto& r = myTextureFactory->LoadTexture(mySelectedAsset.path().wstring().c_str());
	//	auto& image = r ? r : myUnknownArtAssetIcon;
	//	myDragCommand->Execute(&image);
	//	mySelectedAsset = Directory();
	//}


	RenderFolderStructure();
	RenderFolderContents();
	myCreateFolderWindow->Execute(&mySelectedFolderDirToEdit);
	myDeleteFolderCommand->ApplyCommand();
	myCreateFolderCommand->ApplyCommand();


	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && mySelectedAsset != Directory() && IsBeingInteracted())
	{
		if (isImage)
		{
			auto obj = mySceneEditor->GetInspectedObject();
			if (obj)
			{
				auto& r = myTextureFactory->LoadTexture(mySelectedAsset.path().wstring().c_str());
				auto& texture = r ? r : myUnknownArtAssetIcon;

				auto renderer = obj->GetComponent<ModelRenderer>();
				renderer->Model()->myTexture = texture;
			}

		}

		mySelectedAsset = Directory();
	}





}

void Dragonite::AssetBrowser::OnEnable()
{
}

void Dragonite::AssetBrowser::OnDisable()
{
}

const bool Dragonite::AssetBrowser::IsBeingInteracted()
{
	return ImGui::IsWindowHovered() ||
		ImGui::IsWindowFocused() ||
		ImGui::IsAnyItemFocused() ||
		myFolderContentFocus ||
		myFolderStructureFocus;
}

void Dragonite::AssetBrowser::RenderFolderStructure()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar;

	constexpr const char* id = "Assets";
	ImGui::BeginChild(id, ImVec2(250, 0), false, flags);



	if (ImGui::BeginMenuBar())
	{
		ImGui::LabelText("##FolderAssets", myAssetBrowserDirectory.path().string().c_str());
		myFolderMenu->ParentTo("##FolderAssets");
		myFolderMenu->Execute(&myAssetBrowserDirectory);
		ImGui::EndMenuBar();
		myFolderStructureFocus = ImGui::IsWindowHovered() ||
			ImGui::IsWindowFocused() ||
			ImGui::IsAnyItemFocused() ||
			ImGui::IsItemFocused();
	}

	myFolderStructureFocus = ImGui::IsWindowHovered() ||
		ImGui::IsWindowFocused() ||
		ImGui::IsAnyItemFocused() ||
		ImGui::IsItemFocused();

	RenderFolderStructure(myAssetBrowserDirectory);
	ImGui::EndChild();
	ImGui::SameLine();
}

const bool ContainsDirectoryIn(std::filesystem::directory_entry anEntry)
{

	for (auto& entry : std::filesystem::directory_iterator(anEntry))
	{
		if (entry.is_directory())
			return true;
	}
	return false;
}

void Dragonite::AssetBrowser::RenderFolderStructure(Directory anEntry, const bool anIndentFlag)
{
	static bool isSelected = false;
	if (anIndentFlag)
		ImGui::Indent();
	for (auto& entry : DirectoriesOf(anEntry))
	{
		if (entry.is_directory())
		{
			ImGui::PushID(entry.path().filename().string().c_str());

			if (ContainsDirectoryIn(entry))
			{
				if (ImGui::ImageButton(entry.path().string().c_str(), myFolderIcon->GetData().Get(), ImVec2(16, 16)))
				{
					myTreeState[entry.path().string()] = myTreeState.contains(entry.path().string()) ? !myTreeState[entry.path().string()] : true;
				}

			}
			else
			{
				ImGui::Image(myFolderIcon->GetData().Get(), ImVec2(16, 16));
			}
			ImGui::SameLine();

			ImGui::Selectable(entry.path().filename().string().c_str(), &isSelected);

			if (isSelected)
				mySelectedFolderDir = entry;
			isSelected = false;
			auto e = entry;
			myFolderMenu->Execute(&e);
			ImGui::PopID();
			if (myTreeState[entry.path().string()])
				RenderFolderStructure(entry, true);

		}
	}
	if (anIndentFlag)
		ImGui::Unindent();
}

void Dragonite::AssetBrowser::RenderFolderContents()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_None | ImGuiWindowFlags_MenuBar;
	constexpr const char* id = "Browser";



	ImGui::BeginChild(id, ImVec2(0, 0), false, flags);

	myFolderContentFocus = ImGui::IsWindowHovered() ||
		ImGui::IsWindowFocused() ||
		ImGui::IsAnyItemFocused();

	if (ImGui::BeginMenuBar())
	{
		auto dirs = GetAllDirectoriesOfPath(mySelectedFolderDir.path().string());

		Directory prevDir = myAssetBrowserDirectory;
		for (auto& dir : dirs)
		{
			auto path = dir.path().string();
			auto prevPath = prevDir.path().string();

			if (prevDir == myAssetBrowserDirectory)
				path = prevPath + "\\";
			else
			{
				size_t pos = path.find(prevPath);
				if (pos != std::string::npos)
				{
					path.erase(pos, prevPath.length());
				}

			}


			static bool selected;
			ImGui::Selectable(path.c_str(), &selected, ImGuiSelectableFlags_None, ImVec2(7 * path.length(), 0));
			myFolderMenu->Execute(&mySelectedFolderDir);
			ImGui::SameLine();
			if (selected)
				mySelectedFolderDir = dir;

			selected = false;
			prevDir = dir;
		}

		ImGui::EndMenuBar();
	}

	myFolderMenu->Execute(&mySelectedFolderDir);
	RenderFolderContents(mySelectedFolderDir, false);
	ImGui::EndChild();
}

void Dragonite::AssetBrowser::RenderFolderContents(Directory anEntry, const bool anIndentFlag, const int aDepth)
{
	if (!anEntry.exists()) return;

	ImGuiStyle& style = ImGui::GetStyle();
	float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
	int count = 0;






	int fileCount = std::distance(DirectoriesOf(anEntry), DirectoriesOf{});


	ImVec2 iconSize = { 64,64 };

	for (auto& file : DirectoriesOf(anEntry))
	{
		ImGui::PushID(count);
		auto f = file;
		auto extension = file.path().filename().extension();
		ShaderResourceV image;

		bool isDDS = extension == ".dds";
		bool isShader = extension == ".cso";
		bool isDirectory = file.is_directory();
		bool isImage = isDDS || extension == ".png";



		if (isImage)
		{
			auto r = myTextureFactory->LoadTexture(file.path().wstring().c_str());
			image = r ? r->GetData() : myUnknownArtAssetIcon->GetData();
		}
		else if (isShader)
		{
			image = myUnknownArtAssetIcon->GetData();
		}
		else if (isDirectory)
		{
			image = myFolderIcon->GetData();

		}
		else
		{
			image = myJsonIcon->GetData();
		}



		ImGui::BeginGroup();
		{
			if (ImGui::ImageButton(file.path().string().c_str(), image.Get(), iconSize))
			{
				if (file.is_directory())
				{
					mySelectedFolderDir = file;
					myTreeState[file.path().string()] = true;
				}

				else
					mySelectedAsset = file;
			}
			myFolderMenu->Execute(&f);
			auto str = file.path().filename().string();

			std::string name;
			int count = 0;
			for (size_t i = 0; i < str.length(); i++)
			{
				float letterPos = (count + 1) * ImGui::GetFontSize() / 2.0f;
				if (letterPos >= iconSize.x)
				{
					count = 0;
					name += "\n";
				}

				name += str[i];
				count++;
			}


			ImGui::Text(name.c_str());
		}
		ImGui::EndGroup();
		float last_button_x2 = ImGui::GetItemRectMax().x;
		float next_button_x2 = last_button_x2 + style.ItemSpacing.x + iconSize.x; // Expected positision
		if (count + 1 < fileCount && next_button_x2 < window_visible_x2)
			ImGui::SameLine();


		ImGui::PopID();
		count++;
	}
}



std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}


std::vector<Dragonite::AssetBrowser::Directory> Dragonite::AssetBrowser::GetAllDirectoriesOfPath(Path aPath)
{
	std::vector<Directory> r;
	std::string delimiter = "\\";

	/*auto workDirs = split(aWorkingDir.path().string(), delimiter);*/



	auto dirs = split(aPath.string(), delimiter);

	if (dirs.size() <= 1)
	{
		Directory rDir;
		rDir.assign(aPath);
		r.push_back(rDir);
		return r;
	}


	for (size_t i = 0; i < dirs.size(); i++)
	{
		std::string finalDir;
		for (size_t d = 0; d < i + 1; d++)
		{
			finalDir += dirs[d] + "\\";
		}
		Directory rDir;
		rDir.assign(finalDir);
		r.push_back(rDir);

	}




	return r;

}


