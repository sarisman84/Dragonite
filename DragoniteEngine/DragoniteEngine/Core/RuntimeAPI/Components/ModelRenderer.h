#pragma once
#include <memory>
#include "Core/RuntimeAPI/Component.h"
#include "Core/Graphics/Models/Material.h"
#include "Core/RuntimeAPI/Object.h"

#include "Core/Utilities/Reflection.h"

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
		void OnDisable() override;
		void ConstantUpdate() override;
		void OnCreate() override;
		inline std::shared_ptr<ModelInstance>& Model() { return myModelInstance; }
	private:
		ModelFactory* myModelFactory;
		GraphicalInterface* myRenderInterface;
		std::shared_ptr<ModelInstance> myModelInstance;

		std::string GetName() override;
	};


	template<>
	inline auto Reflect::RegisterElement<ModelRenderer>()
	{
		return Class("Model Renderer");
	}
}


