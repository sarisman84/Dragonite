#pragma once
#include "Core/RuntimeAPI/Component.h"
#include "Core/CU/Math/Vector2.h"

#include <memory>

namespace Dragonite
{
	class InputManager;
	class SpriteRenderer;
	class Mouse;
	class PlayerController : public Component
	{
	public:
		// Inherited via Component
		void Awake() override;
		void Start() override;
		void Update(const float aDt) override;
		void LateUpdate(const float aDt) override;
		void* Serialize() override;
		void Deserialize(void* someData) override;
		void OnInspectorGUI() override;
		std::shared_ptr<Component> Clone() override;

		float myMovementSpeed;

	private:
		SpriteRenderer* mySprite;
		InputManager* myInputManager;
		Mouse* myMousePtr;
		Vector2f myTargetPosition;

		// Inherited via Component

	};
}


