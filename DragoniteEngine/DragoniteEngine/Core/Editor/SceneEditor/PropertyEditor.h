#pragma once
#include "Core/Editor/GUIWindow.h"
#include <unordered_map>
#include <typeinfo>
#include <string>


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
		template<typename TComponent>
		void RegisterComponent();


		void AddComponent(Object* anObjectToWorkWith, std::string aCompName);
		void InspectComponent(Component* aComponent);


		template<typename Type>
		const bool IsOfType(const int aHashCode);

		SceneEditor* mySceneEditor;

		std::unordered_map<std::string, int> myRegisteredComponents;

	};
	template<typename TComponent>
	inline void PropertyEditor::RegisterComponent()
	{
		auto& info = typeid(TComponent);
		int key = static_cast<int>(info.hash_code());
		std::string name = info.name();
		myRegisteredComponents[name] = key;
	}
	template<typename Type>
	inline const bool PropertyEditor::IsOfType(const int aHashCode)
	{
		int incomingHashCode = static_cast<int>(typeid(Type).hash_code());
		return aHashCode == incomingHashCode;
	}
}


