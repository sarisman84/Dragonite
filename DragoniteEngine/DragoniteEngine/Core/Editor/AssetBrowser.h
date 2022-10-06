#pragma once
#include "GUIWindow.h"

#include <filesystem>
#include <string>



namespace Dragonite
{
	class TextureFactory;
	class ModelInstance;
	class Scene;
	class SceneEditor;
	class Texture;

	class ModalWindow;
	class FolderCommand;
	class PopupMenu;

	class AssetBrowser : public GUIWindow
	{
		typedef std::filesystem::directory_entry Directory;
		typedef std::filesystem::directory_iterator DirectoriesOf;
		typedef std::filesystem::path Path;
	public:
		AssetBrowser();
		void OnWindowInit() override;
		void OnWindowRender() override;
		void OnEnable() override;
		void OnDisable() override;
	private:
		void RenderFolderStructure();
		void RenderFolderStructure(Directory anEntry, const bool anIndentFlag = false);
		
		void RenderFolderContents();
		void RenderFolderContents(Directory anEntry, const bool anIndentFlag = false, const int aDepth = 0);
		
		void DisplayFolderCommands(Directory anDirectoryToEdit);




		void DefineFolderCommands();


		std::unique_ptr<ModalWindow> myCreateFolderWindow;
		std::unique_ptr<FolderCommand> myCreateFolderCommand, myDeleteFolderCommand;
		std::unique_ptr<PopupMenu> myFolderMenu;

		Directory mySelectedFolderDirToEdit;
		Directory mySelectedFolderDir;
		Directory myAssetBrowserDirectory;

		std::shared_ptr<Texture>
			myFolderIcon,
			myShaderIcon,
			myUnknownArtAssetIcon,
			myJsonIcon;

		TextureFactory* myTextureFactory;
		Scene* myScene;
		SceneEditor* mySceneEditor;
	};
}


