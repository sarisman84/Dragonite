#pragma once

#include <unordered_map>
#include <functional>
#include "Core/External/nlohmann/json.hpp"


namespace Dragonite
{
	class Object;
	class Scene;
	struct BuildData
	{
		Scene& myCurrentScene;
		Object& myObject;
	};


	typedef std::function<void(const nlohmann::json& anInput, const BuildData&)> ImportSetting;
	typedef std::function<void(nlohmann::json& anOutput, const BuildData&)> ExportSetting;
	void InitializeImportSettings(std::unordered_map<std::string, ImportSetting>& someImportSettings);
	void InitializeExportSettings(std::unordered_map<std::string, ExportSetting>& someExportSettings);
}