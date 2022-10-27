#pragma once
#include "Core/RuntimeAPI/Component.h"
#include "Core/Utilities/Reflection.h"

#include <memory>
namespace Dragonite
{
	class TestComponent : public Component
	{
	public:
		// Inherited via Component
		void Awake() override;
		void Update(const float aDt) override;
		void Start() override;
		void LateUpdate(const float aDt) override;
		void* Serialize() override;
		void Deserialize(void* someData) override;
		void OnInspectorGUI() override;	
		std::shared_ptr<Dragonite::Component> Clone() override;
	public:
		float
			myScaleSpeed,
			myMinScale;

	private:
		float myCurrentTime;

		// Inherited via Component
	

	};


	template<>
	inline auto Reflect::RegisterElement<TestComponent>()
	{
		return Class("Test Component",
			Member("Scale Speed", &TestComponent::myScaleSpeed),
			Member("Min Scale", &TestComponent::myMinScale)
		);
	}

}

