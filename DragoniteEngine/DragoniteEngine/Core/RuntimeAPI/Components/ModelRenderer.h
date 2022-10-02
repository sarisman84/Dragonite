#pragma once
#include <memory>
#include "Core/RuntimeAPI/Component.h"
#include "Core/Graphics/Models/Material.h"

namespace Dragonite
{
	class ModelFactory;
	class ModelInstance;
	class ModelRenderer : public Component
	{
	public:
		void Awake() override;
		void Update(const float aDt) override;
	private:
		ModelFactory* myModelFactory;
		std::shared_ptr<ModelInstance> myModel;
		Material myMaterial;
	};
}


