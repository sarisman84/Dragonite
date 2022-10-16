#pragma once
#include "Core/RuntimeAPI/Component.h"
#include "Core/Utilities/Reflection.h"
namespace Dragonite
{
	class TestComponent : public Component
	{
	public:
		// Inherited via Component
		void Awake() override;
		void Update(const float aDt) override;
		void OnInspectorGUI() override;
		void OnDisable() override;
		void ConstantUpdate() override;
		void OnCreate() override;
		std::string GetName() override;

	public:
		float
			myScaleSpeed,
			myMinScale;

	private:
		float myCurrentTime;
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

