#pragma once
#include "Core/RuntimeAPI/Component.h"
#include "Core/Graphics/Models/Material.h"
#include "Core/RuntimeAPI/NEW/Object.h"


#include "Core/CU/Math/Matrix4x4.hpp"

#include <memory>

namespace Dragonite
{
	class GraphicalInterface;
	class ModelFactory;
	class ModelInstance;
	class SpriteRenderer : public Component
	{
	public:
		void Awake() override;
		void Start() override;
		void Update(const float aDt) override;
		void LateUpdate(const float aDt) override;
		void* Serialize() override;
		void Deserialize(void* someData) override;
		void OnInspectorGUI() override;

		Matrix4x4f GetLocal2DMatrix();


		std::shared_ptr<ModelInstance>& Sprite() { return mySpriteInstance; }

		Vector2f myPivot;
		Vector2f myUVMin;
		Vector2f myUVMax;

	private:
		GraphicalInterface* myInterface;
		ModelFactory* myModelFactory;
		std::shared_ptr<ModelInstance> mySpriteInstance;

		// Inherited via Component
		virtual std::shared_ptr<Component> Clone() override;
	};
}