#include "SceneEditor.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/Graphics/GraphicsAPI.h"
#include <string>

#include "Core/External/imgui/misc/cpp/imgui_stdlib.h"
#include "Core/Graphics/RenderTargets/RenderFactory.h"


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

Dragonite::SceneEditor::SceneEditor() : GUIWindow("Hierachy")
{

}

void Dragonite::SceneEditor::OnWindowRender()
{

	if (!myCurrentScene) return;

	if (ImGui::Button("Add Object"))
	{
		//myCurrentScene->SceneObjects().push_back(myModelFactory->GetModel(PrimitiveType::Cube, Material::defaultMaterial));

		Object newObject = Object("New GameObject");
		newObject.Init(myPollingStation);
		newObject.GetTransform().myPosition = { 0,0, 1 };

		auto modelRenderer = newObject.AddComponent<ModelRenderer>();

		modelRenderer->Model() = myModelFactory->GetModel(PrimitiveType::Cube, Material::defaultMaterial);


		myCurrentScene->SceneObjects().push_back(newObject);
		mySelectedElements.push_back(false);
	}



	{
		std::string name = "Camera";
		name += std::string("##") + std::to_string(0);
		static bool val;
		ImGui::Selectable(name.c_str(), &val);
		if (val)
			FocusElement(0);


		int index = 0;

		auto& sceneObjs = myCurrentScene->SceneObjects();
		for (size_t i = 0; i < sceneObjs.size(); i++)
		{
			val = false;
			auto& object = sceneObjs[i];
			std::string name = sceneObjs[i].Name();
			name += std::string("##") + std::to_string(i);
			ImGui::Selectable(name.c_str(), &val);
			if (val)
				FocusElement(i + 1);
			val = false;

		}
	}







	ImGui::Begin("Property");
	if (IsInspectingFocusedElement())
	{
		if (myFocusedElement == 0)
			InspectCamera(myCurrentScene->GetCamera());
		else
			InspectFocusedElement(myCurrentScene);
	}
	ImGui::End();


}

void Dragonite::SceneEditor::OnEnable()
{
}

void Dragonite::SceneEditor::OnDisable()
{
}

void Dragonite::SceneEditor::FocusElement(const int anElementToFocus)
{
	if (anElementToFocus < 0 || anElementToFocus >= mySelectedElements.size()) return;

	for (size_t i = 0; i < mySelectedElements.size(); i++)
	{
		mySelectedElements[i] = false;
	}


	mySelectedElements[anElementToFocus] = true;
	myFocusedElement = anElementToFocus;
}

const bool Dragonite::SceneEditor::IsInspectingFocusedElement()
{
	return myFocusedElement < 0 || myFocusedElement >= mySelectedElements.size() ? false : mySelectedElements[myFocusedElement];
}

void Dragonite::SceneEditor::InspectFocusedElement(Scene* aScene)
{
	int focusedElement = myFocusedElement - 1;
	auto& sceneObjs = aScene->SceneObjects();
	std::string name = sceneObjs[focusedElement].Name();
	name += std::string("##") + std::to_string(focusedElement);
	ImGui::InputText("Name", &sceneObjs[focusedElement].Name(), 0, DefaultStringResize);
	if (ImGui::BeginListBox(std::string("##" + name).c_str(), ImVec2(300, 250)))
	{


		std::string name = "Transform";
		name += std::string("##") + sceneObjs[focusedElement].Name() + std::to_string(focusedElement);
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::Indent();
			ImGui::DragFloat3("Position", (float*)&sceneObjs[focusedElement].GetTransform().myPosition, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&sceneObjs[focusedElement].GetTransform().myRotation, 0.1f);
			ImGui::DragFloat3("Size", (float*)&sceneObjs[focusedElement].GetTransform().myScale, 0.1f);
			ImGui::Unindent();
		}

		int compIndex = 0;
		for (auto& comp : sceneObjs[focusedElement].Components())
		{
			name = "Component";
			name += std::to_string(compIndex++);
			name += std::string("##") + sceneObjs[focusedElement].Name() + std::to_string(focusedElement);
			if (ImGui::CollapsingHeader(name.c_str()))
			{
				ImGui::Indent();
				comp->OnInspectorGUI();
				ImGui::Unindent();
			}

		}


		ImGui::EndListBox();

	}
}

void Dragonite::SceneEditor::InspectCamera(Camera& aCamera)
{
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
			ImGui::DragFloat3("Rotation", (float*)&cam.GetTransform().myRotation, 0.1f);
			ImGui::Unindent();
		}

		cmpName = "Camera";
		cmpName += std::string("##") + name;
		if (ImGui::CollapsingHeader(cmpName.c_str()))
		{

			auto perspectiveProfile = dynamic_cast<PerspectiveProfile*>(cam.GetProfile());

			ImGui::Indent();
			ImGui::DragFloat("FOV", (float*)&perspectiveProfile->myFOV, 0.1f);
			ImGui::DragFloat("Near Plane", (float*)&perspectiveProfile->myNearPlane, 0.1f);
			ImGui::DragFloat("Far Plane", (float*)&perspectiveProfile->myFarPlane, 0.1f);
			ImGui::Checkbox("View Render ID", &myRenderID.ViewRenderID());
			ImGui::Unindent();
		}



		ImGui::EndListBox();
	}
}

void Dragonite::SceneEditor::OnWindowInit()
{
	mySelectedElements.push_back(false); //Add Camera here
	myRenderID = RenderID(myPollingStation->Get<GraphicsPipeline>());

	myCurrentScene = myPollingStation->Get<Scene>();
	myModelFactory = myPollingStation->Get<ModelFactory>();

	myRenderID.SetupRenderID(myCurrentScene);
	myPollingStation->Get<RenderFactory>()->RegisterTarget(myRenderID);


}



