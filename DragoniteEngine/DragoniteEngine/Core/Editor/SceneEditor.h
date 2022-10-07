#pragma once
#include "GUIWindow.h"
#include "Core/Editor/RenderID.h"
#include <functional>

namespace Dragonite
{
	class ModelFactory;
	class Scene;
	class Camera;
	class Mouse;
	class Object;
	class SceneEditor :public GUIWindow
	{
	public:
		SceneEditor();

		void OnWindowRender() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnWindowInit() override;
		const bool IsBeingInteracted() override;

		inline RenderID& GetSceneObjectID() { return myRenderID; }
		inline Mouse* GetMouseInput() { return myMouseInput; }
		Object* GetInspectedObject();
		const bool IsInspectingFocusedElement();
	private:
		void FocusElement(const int anElementToFocus);
		void ResetFocus();



		void InspectFocusedElement(Scene* aScene);
		void InspectCamera(Camera& aCamera);

		bool myPropertyFocus;

		int myFocusedElement;
		std::vector<bool> mySelectedElements;
		ModelFactory* myModelFactory;
		Scene* myCurrentScene;
		RenderID myRenderID;
		Mouse* myMouseInput;



	};

}

