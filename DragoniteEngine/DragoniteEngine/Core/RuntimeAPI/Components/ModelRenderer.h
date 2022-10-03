#pragma once
#include <memory>
#include "Core/RuntimeAPI/Component.h"
#include "Core/Graphics/Models/Material.h"
#include "Core/RuntimeAPI/Object.h"

namespace Dragonite
{
	class RenderInterface;
	class ModelFactory;
	class ModelInstance;
	class ModelRenderer : public Component
	{
	public:
		void Awake() override;
		void Update(const float aDt) override;
		void OnInspectorGUI() override;
		inline std::shared_ptr<ModelInstance>& Model() { return myModel; }
	private:
		ModelFactory* myModelFactory;
		RenderInterface* myRenderInterface;
		std::shared_ptr<ModelInstance> myModel;

	
	};
}


