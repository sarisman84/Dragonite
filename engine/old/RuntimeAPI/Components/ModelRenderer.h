#pragma once
#include <memory>
#include "Core/RuntimeAPI/Component.h"
#include "Core/Graphics/Models/Material.h"
#include "Core/RuntimeAPI/NEW/Object.h"

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
		void LateUpdate(const float aDt) override;
		void Start() override;
		void* Serialize() override;
		void Deserialize(void* someData) override;
		void OnInspectorGUI() override;


		inline std::shared_ptr<ModelInstance>& Model() { return myModelInstance; }
	private:
		ModelFactory* myModelFactory;
		GraphicalInterface* myRenderInterface;
		std::shared_ptr<ModelInstance> myModelInstance;






		// Inherited via Component
		virtual std::shared_ptr<Component> Clone() override;

	};


	template<>
	inline auto Reflect::RegisterElement<ModelRenderer>()
	{
		return Class("Model Renderer");
	}
}


