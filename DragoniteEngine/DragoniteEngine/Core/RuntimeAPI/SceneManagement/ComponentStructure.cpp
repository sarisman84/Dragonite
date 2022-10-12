#include "ComponentStructure.h"
#include "Core/RuntimeAPI/Object.h"
#include "Core/RuntimeAPI/Scene.h"

#include "Core/RuntimeAPI/Components/ModelRenderer.h"
#include "Core/Graphics/Models/ModelFactory.h"

#include "Core/PollingStation.h"
#include "Core/Runtime.h"
#include "Core/Graphics/GraphicsAPI.h"

void Dragonite::InitializeImportSettings(std::unordered_map<std::string, ImportSetting>& someImportSettings)
{
	someImportSettings["modelrenderer"] = [](const nlohmann::json& anInput, const BuildData& someBuildData)
	{
		auto pollingStation = someBuildData.myCurrentScene.GetPollingStation();
		auto mr = someBuildData.myObject.AddComponent<ModelRenderer>();


		auto model = anInput["model"].get<std::string>();
		auto material = anInput["material"].get<std::string>();
		auto texture = std::filesystem::path(anInput["texture"].get<std::string>()).wstring();

		mr->Model() = pollingStation->Get<ModelFactory>()->GetModel(model, material, texture);
	};

}

void Dragonite::InitializeExportSettings(std::unordered_map<std::string, ExportSetting>& someExportSettings)
{

	someExportSettings["modelrenderer"] = [](nlohmann::json& anOutput, const BuildData& someBuildData)
	{
		auto mr = someBuildData.myObject.GetComponent<ModelRenderer>();
		auto& model = mr->Model();
		anOutput["model"] = model->myModelName;
		anOutput["texture"] = model->myTexture->Name().c_str();
		anOutput["material"] = model->myMaterialName;
	};
}
