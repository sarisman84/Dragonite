#pragma once
#include <memory>
#include <vector>
#include "Core/Editor/GUIWindow.h"


namespace Dragonite
{
	class Scene;
	class Runtime;
	class GraphicalInterface;

	class DragoniteGui
	{
		friend GUIWindow;
	public:
		DragoniteGui();
		~DragoniteGui();
		void Init(Runtime* anAppIns, GraphicalInterface* aGraphicsPipeline);
		void Render();

		void FocusScene(Scene* aNewScene);
		Scene* GetFocusedScene();

		void CreateEditorWindow(GUIWindow* aWindowType);
		std::unique_ptr<GUIWindow>& GetWindow(const char* aName);

	private:
		void InitializeImgui();
		void BeginDockingSpace();
		void EndDockingSpace();


		Scene* myFocusedScene;

		bool myIsCurrentlyOnImGuiFlag;

		std::vector<std::unique_ptr<GUIWindow>> myGuiWindows;
		Runtime* myApplicationIns;
		GraphicalInterface* myGraphicsPipeline;
	};
}


