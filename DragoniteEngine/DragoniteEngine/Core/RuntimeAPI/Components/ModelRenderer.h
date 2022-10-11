#pragma once
#include <memory>
#include "Core/RuntimeAPI/Component.h"
#include "Core/Graphics/Models/Material.h"
#include "Core/RuntimeAPI/Object.h"

namespace Dragonite
{
	class GraphicalInterface;
	class ModelFactory;
	class ModelInstance;
	class ModelRenderer : public Component
	{
	public:
		void Awake() override;
		void Update(const float aDt) override;
		void OnInspectorGUI() override;
		inline std::shared_ptr<ModelInstance>& Model() { return myModelInstance; }
	private:
		ModelFactory* myModelFactory;
		GraphicalInterface* myRenderInterface;
		std::shared_ptr<ModelInstance> myModelInstance;

	

		// Inherited via Component
		virtual std::string GetName() override;

	};
}


