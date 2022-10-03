#include "SceneEditor.h"
#include "Core/RuntimeAPI/Scene.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include <string>

#include "Core/External/imgui/misc/cpp/imgui_stdlib.h"


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
	//Lazy load the nessesary elements
	myCurrentScene = !myCurrentScene ? myPollingStation->Get<Scene>() : myCurrentScene;
	myModelFactory = !myModelFactory ? myPollingStation->Get<ModelFactory>() : myModelFactory;

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

	ImGui::BeginListBox("Objects");
	int index = 0;
	bool isSomethingSelected = false;
	auto& sceneObjs = myCurrentScene->SceneObjects();
	for (size_t i = 0; i < sceneObjs.size(); i++)
	{
		bool val = mySelectedElements[i];
		auto& object = sceneObjs[i];
		std::string name = sceneObjs[i].Name();
		name += std::string("##") + std::to_string(i);
		ImGui::Selectable(name.c_str(), &val);
		mySelectedElements[i] = val;

		if (mySelectedElements[i] && !isSomethingSelected)
			isSomethingSelected = true;
	}

	ImGui::EndListBox();



	if (isSomethingSelected)
	{
		ImGui::Begin("Property");
		for (size_t i = 0; i < mySelectedElements.size(); i++)
		{
			if (mySelectedElements[i])
			{
				std::string name = sceneObjs[i].Name();
				name += std::string("##") + std::to_string(i);
				if (ImGui::CollapsingHeader(name.c_str()))
				{
					ImGui::InputText("Name", &sceneObjs[i].Name(), 0, DefaultStringResize);
					ImGui::Indent();
					std::string name = "Transform";
					name += std::string("##") + sceneObjs[i].Name() + std::to_string(i);
					if (ImGui::CollapsingHeader(name.c_str()))
					{
						ImGui::Indent();
						ImGui::DragFloat3("Position", (float*)&sceneObjs[i].GetTransform().myPosition, 0.1f);
						ImGui::DragFloat3("Rotation", (float*)&sceneObjs[i].GetTransform().myRotation, 0.1f);
						ImGui::DragFloat3("Size", (float*)&sceneObjs[i].GetTransform().myScale, 0.1f);
						ImGui::Unindent();
					}

					int compIndex = 0;
					for (auto& comp : sceneObjs[i].Components())
					{
						name = "Component";
						name += std::to_string(compIndex++);
						name += std::string("##") + sceneObjs[i].Name() + std::to_string(i);
						if (ImGui::CollapsingHeader(name.c_str()))
						{
							ImGui::Indent();
							comp->OnInspectorGUI();
							ImGui::Unindent();
						}

					}

					ImGui::Unindent();
				}


			}
		}
		ImGui::End();
	}

}

void Dragonite::SceneEditor::OnEnable()
{
}

void Dragonite::SceneEditor::OnDisable()
{
}



