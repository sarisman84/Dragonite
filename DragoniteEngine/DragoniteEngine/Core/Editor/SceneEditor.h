#pragma once
#include "GUIWindow.h"
#include "Core/Editor/RenderID.h"
namespace Dragonite
{
	class ModelFactory;
	class Scene;
	class Camera;

	class SceneEditor :public GUIWindow
	{
	public:
		SceneEditor();

		void OnWindowRender() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnWindowInit() override;

	private:
		void FocusElement(const int anElementToFocus);
		const bool IsInspectingFocusedElement();


		void InspectFocusedElement(Scene* aScene);
		void InspectCamera(Camera& aCamera);

		int myFocusedElement;
		std::vector<bool> mySelectedElements;
		ModelFactory* myModelFactory;
		Scene* myCurrentScene;
		RenderID myRenderID;


	};

}

