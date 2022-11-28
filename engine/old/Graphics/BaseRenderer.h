#pragma once
#include <vector>
#include <memory>
namespace Dragonite
{
	class ModelInstance;
	class BaseRenderer
	{
	public:
		virtual void Render(std::vector<std::shared_ptr<ModelInstance>> someInstancesToRender) = 0;
	};

}