#include "SceneEditor.h"
#include "Core/Scene.h"
#include "Core/Graphics/Models/ModelFactory.h"
#include <string>


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
		myCurrentScene->SceneObjects().push_back(myModelFactory->GetModel(PrimitiveType::Cube, Material::defaultMaterial));
	}

	ImGui::BeginListBox("Objects");
	int index = 0;
	for (auto& object : myCurrentScene->SceneObjects())
	{
		bool selected_tmp = false;
		std::string name = "Object";
		name += "_" + std::to_string(index++);
		ImGui::Selectable(name.c_str(), &selected_tmp);
	}
	ImGui::EndListBox();

}

void Dragonite::SceneEditor::OnEnable()
{
}

void Dragonite::SceneEditor::OnDisable()
{
}
