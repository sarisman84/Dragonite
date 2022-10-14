#pragma once
#include "Core/Editor/GUIWindow.h"
#include <unordered_map>
#include <typeinfo>
#include <string>
#include "Core/Utilities/Reflection.h"


int DefaultStringResize(ImGuiInputTextCallbackData* data);

namespace Dragonite
{
	class Object;
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
		SceneEditor* mySceneEditor;

	

	};

}


