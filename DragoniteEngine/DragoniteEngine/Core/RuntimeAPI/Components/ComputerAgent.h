#pragma once
#include "Core/RUntimeAPI/Component.h"

#include "Core/CU/Math/Vector.h"

namespace Dragonite
{
	class SpriteRenderer;

	class ComputerAgent : public Component 
	{
	public:
		void Awake() override;
		void Start() override;
		void Update(const float aDt) override;
		void LateUpdate(const float aDt) override;
		void* Serialize() override;
		void Deserialize(void* someData) override;
		void OnInspectorGUI() override;
		std::shared_ptr<Component> Clone() override;

		float myMovementSpeed;
		bool myPollInformationFlag;

	private:
		SpriteRenderer* mySprite;
		Vector2f myTargetPosition;
		bool myHasReachedDestination;
	};

}