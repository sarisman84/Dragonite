#include "PropertyEditor.h"
#include "Core/Editor/SceneEditor.h"
#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Component.h"
#include "Core/RuntimeAPI/Components/TestComponent.h"

#include "Core/External/imgui/misc/cpp/imgui_stdlib.h"

#pragma region OLD CODE

/*

std::string name = "Camera";
	name += std::string("##") + std::to_string(0);
	if (ImGui::BeginListBox(std::string("##" + name).c_str(), ImVec2(300, 250)))
	{

		auto& cam = aCamera;
		std::string cmpName = "Transform";
		cmpName += std::string("##") + name;
		if (ImGui::CollapsingHeader(cmpName.c_str()))
		{


			ImGui::Indent();
			ImGui::DragFloat3("Position", (float*)&cam.GetTransform().myPosition, 0.1f);
			myPropertyFocus = myPropertyFocus || ImGui::IsItemFocused() || ImGui::IsItemHovered();
			ImGui::DragFloat3("Rotation", (float*)&cam.GetTransform().myRotation, 0.1f);
			myPropertyFocus = myPropertyFocus || ImGui::IsItemFocused() || ImGui::IsItemHovered();
			ImGui::Unindent();
		}
		myPropertyFocus = myPropertyFocus || ImGui::IsItemFocused() || ImGui::IsItemHovered();

		cmpName = "Camera";
		cmpName += std::string("##") + name;
		if (ImGui::CollapsingHeader(cmpName.c_str()))
		{

			auto perspectiveProfile = dynamic_cast<PerspectiveProfile*>(cam.Profile());

			ImGui::Indent();
			ImGui::DragFloat("FOV", (float*)&perspectiveProfile->myFOV, 0.1f);
			myPropertyFocus = myPropertyFocus || ImGui::IsItemFocused() || ImGui::IsItemHovered();
			ImGui::DragFloat("Near Plane", (float*)&perspectiveProfile->myNearPlane, 0.1f);
			myPropertyFocus = myPropertyFocus || ImGui::IsItemFocused() || ImGui::IsItemHovered();
			ImGui::DragFloat("Far Plane", (float*)&perspectiveProfile->myFarPlane, 0.1f);
			myPropertyFocus = myPropertyFocus || ImGui::IsItemFocused() || ImGui::IsItemHovered();
			//ImGui::Checkbox("View Render ID", &myRenderID.ViewRenderID());
			ImGui::Unindent();
		}
		myPropertyFocus = myPropertyFocus || ImGui::IsItemFocused() || ImGui::IsItemHovered();



		ImGui::EndListBox();
	}
*/


#pragma endregion



int DefaultStringResize(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		ImVector<char>* my_str = (ImVector<char>*)data->UserData;
		IM_ASSERT(my_str->begin() == data->Buf);
		my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
		data->Buf = my_str->begin();
	}
	return 0;
}


Dragonite::PropertyEditor::PropertyEditor() : GUIWindow("Inspector")
{
}

Dragonite::PropertyEditor::~PropertyEditor()
{
	
}

void Dragonite::PropertyEditor::OnWindowInit()
{
	RegisterComponent<TestComponent>();
}

void Dragonite::PropertyEditor::OnWindowUpdate()
{
	if (!mySceneEditor) return;


	Object* selectedObject = mySceneEditor->GetInspectedObject();

	if (!selectedObject) return;


	ImGui::InputText("Name", &selectedObject->Name(), 0, DefaultStringResize);
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		ImGui::DragFloat3("Position", (float*)&selectedObject->GetTransform().myPosition, 0.01f);
		ImGui::DragFloat3("Rotation", (float*)&selectedObject->GetTransform().myRotation, 0.01f);
		ImGui::DragFloat3("Size", (float*)&selectedObject->GetTransform().myScale, 0.01f);
		ImGui::Unindent();
	}
	auto components = selectedObject->Components();
	for (size_t i = 0; i < components.size(); i++)
	{
		std::string name(components[i]->GetName());
		name += std::to_string(i);

		if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Indent();
			InspectComponent(components[i].get());
			ImGui::Unindent();
		}
	}


	ImGui::Spacing();



	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("add_component");
	}

	if (ImGui::BeginPopup("add_component"))
	{
		for (auto& comp : myRegisteredComponents)
		{
			if (ImGui::Button(comp.first.c_str()))
			{
				AddComponent(selectedObject, comp.first);
				ImGui::CloseCurrentPopup();
			}
		}
		

		ImGui::EndPopup();
	}




}

void Dragonite::PropertyEditor::OnEnable()
{
}

void Dragonite::PropertyEditor::OnDisable()
{
}

void Dragonite::PropertyEditor::AddComponent(Object* anObjectToWorkWith, std::string aCompName)
{
	auto key = myRegisteredComponents[aCompName];

	if (IsOfType<TestComponent>(key))
	{
		anObjectToWorkWith->AddComponent<TestComponent>();
	}

}

void Dragonite::PropertyEditor::InspectComponent(Component* aComponent)
{
	aComponent->OnInspectorGUI();
}
