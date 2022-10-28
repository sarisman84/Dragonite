//#pragma once
//#include "Core/Editor/GUIWindow.h"
//#include <unordered_map>
//#include <typeinfo>
//#include <string>
//#include "Core/Utilities/Reflection.h"
//#include "Core/CU/Math/Vector2.h"
//
//
//
//int DefaultStringResize(ImGuiInputTextCallbackData* data);
//
//namespace Dragonite
//{
//	class Object;
//	class Component;
//	class SceneEditor;
//	class PropertyEditor :public GUIWindow
//	{
//	public:
//		PropertyEditor();
//		~PropertyEditor() override;
//
//		inline void RegisterSceneEditor(SceneEditor* anEditor) { mySceneEditor = anEditor; }
//
//	protected:
//		// Inherited via GUIWindow
//		void OnWindowInit() override;
//		void OnWindowUpdate() override;
//		void OnEnable() override;
//		void OnDisable() override;
//
//	private:
//		SceneEditor* mySceneEditor;
//		std::vector<std::shared_ptr<Component>> myRegistry;
//
//
//	};
//
//
//	
//
//
//	namespace Reflect
//	{
//		template<typename Member>
//		inline void InspectMember(const char* aName, Member* aMember)
//		{
//
//		}
//
//
//		template<>
//		inline void InspectMember<float>(const char* aName, float* aFloatVal)
//		{
//			ImGui::DragFloat(aName, aFloatVal, 0.1f);
//		}
//
//		template<>
//		inline void InspectMember<bool>(const char* aName, bool* aBoolVal)
//		{
//			ImGui::Checkbox(aName, aBoolVal);
//		}
//
//
//		template<>
//		inline void InspectMember<Vector2f>(const char* aName, Vector2f* aBoolVal)
//		{
//			ImGui::DragFloat2(aName, (float*)aBoolVal);
//		}
//
//
//		template<>
//		inline void InspectMember<const char*>(const char* aName, const char** aConstCharVal)
//		{
//			char input[200];
//
//			strcpy_s<200>(input, *aConstCharVal);
//			ImGui::InputText(aName, input, sizeof(input));
//			*aConstCharVal = input;
//		}
//
//
//
//
//
//
//		template<typename TClass>
//		void InspectElement(TClass* anElement)
//		{
//			IterateMembers(anElement, [anElement](auto member)
//				{
//					Reflect::InspectMember(member.Name(), &member.ValueOf(anElement));
//				}, true);
//		}
//	}
//}
//
//
