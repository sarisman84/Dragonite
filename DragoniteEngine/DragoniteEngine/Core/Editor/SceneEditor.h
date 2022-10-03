#pragma once
#include "GUIWindow.h"
namespace Dragonite
{
	class ModelFactory;
	class Scene;

	class SceneEditor :public GUIWindow
	{
	public:
		SceneEditor();
		// Inherited via GUIWindow
		void OnWindowRender() override;
		void OnEnable() override;
		void OnDisable() override;
	private:
		std::vector<bool> mySelectedElements;
		ModelFactory* myModelFactory;
		Scene* myCurrentScene;
	};

}

