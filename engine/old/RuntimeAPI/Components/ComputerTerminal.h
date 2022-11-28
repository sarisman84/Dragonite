#pragma once

#include "Core/RuntimeAPI/Component.h"

#include "Core/Utilities/Reflection.h"

namespace Dragonite
{
	class SpriteRenderer;
	class ComputerTerminal : public Component
	{
		// Inherited via Component
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update(const float aDt) override;
		virtual void LateUpdate(const float aDt) override;
		virtual void* Serialize() override;
		virtual void Deserialize(void* someData) override;
		virtual void OnInspectorGUI() override;
		virtual std::shared_ptr<Component> Clone() override;

	private:
		SpriteRenderer* mySprite;
	};


	template<>
	inline auto Reflect::RegisterElement<ComputerTerminal>() 
	{
		return Class("Computer Terminal");
	}
}