#pragma once
#include "Core/Editor/GUIWindow.h"

namespace Dragonite
{
	class Component;
	class SceneEditor;
	class PropertyEditor :public GUIWindow
	{
	public:
		PropertyEditor();
		~PropertyEditor() override;

		inline void RegisterSceneEditor(SceneEditor* anEditor) { mySceneEditor = anEditor; }

	protected:
		// Inherited via GUIWindow
		void OnWindowInit() override;
		void OnWindowUpdate() override;
		void OnEnable() override;
		void OnDisable() override;

	private:
		void InspectComponent(Component* aComponent);

		SceneEditor* mySceneEditor;
	};
}


