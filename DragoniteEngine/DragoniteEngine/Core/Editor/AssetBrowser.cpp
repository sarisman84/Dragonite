#include "AssetBrowser.h"
#include "Commands/MenuCommand.h"

#include "Core/Graphics/Textures/TextureFactory.h"
#include "Core/RuntimeAPI/Scene.h"

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


	myScene = myPollingStation->Get<Scene>();
	myTextureFactory = myPollingStation->Get<TextureFactory>();



	myFolderIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/open-folder.dds");
	myUnknownArtAssetIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/large-paint-brush.dds");
	myJsonIcon = myTextureFactory->LoadTexture(L"resources/textures/icons/paper-clip.dds");


	/*myCurrentScene = myPollingStation->Get<Scene>();
	myModelFactory = myPollingStation->Get<ModelFactory>();*/
}

void Dragonite::AssetBrowser::OnWindowRender()
{

	RenderFolderStructure();
	RenderFolderContents();
	myCreateFolderWindow->Execute(&mySelectedFolderDirToEdit);
	myDeleteFolderCommand->ApplyCommand();
	myCreateFolderCommand->ApplyCommand();
}

void Dragonite::AssetBrowser::OnEnable()
{
}

void Dragonite::AssetBrowser::OnDisable()
{
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
	}


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
	static std::unordered_map<std::string, bool> myTreeState;
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

	if (ImGui::BeginMenuBar())
	{
		ImGui::LabelText("##FolderAssets", mySelectedFolderDir.path().string().c_str());
		myFolderMenu->ParentTo("##FolderAssets");
		myFolderMenu->Execute(&mySelectedFolderDir);
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
		if (extension == ".dds" || extension == ".png")
		{
			image = myTextureFactory->LoadTexture(file.path().wstring().c_str())->GetData();
		}
		else if (extension == ".cso")
		{
			image = myUnknownArtAssetIcon->GetData();
		}
		else if (file.is_directory())
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
					mySelectedFolderDir = file;
				else
				{

				}
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

void Dragonite::AssetBrowser::DisplayFolderCommands(Directory anDirectoryToEdit)
{



}

void Dragonite::AssetBrowser::DefineFolderCommands()
{


}
