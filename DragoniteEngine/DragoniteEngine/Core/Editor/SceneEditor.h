#pragma once
#include "GUIWindow.h"
#include <functional>
#include <memory>

namespace Dragonite
{
	class ModelFactory;
	class Scene;
	class Camera;
	class Mouse;
	class Object;
	class Viewport;
	class PropertyEditor;
	class SceneEditor :public GUIWindow
	{
	public:
		SceneEditor();
		~SceneEditor() override;
		void OnWindowUpdate() override;
		void OnEnable() override;
		void OnDisable() override;
		void OnWindowInit() override;

		Object* GetInspectedObject();

		void InitializeNewObject();

	private:
		



		int myFocusedElement;
		ModelFactory* myModelFactory;
		Scene* myCurrentScene;

		Mouse* myMouseInput;
		PropertyEditor* myPropertyEditor;
		Viewport* myViewport;


	};

}

