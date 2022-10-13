#pragma once
#include "Core/RuntimeAPI/Component.h"
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

}

