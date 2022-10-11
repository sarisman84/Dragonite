#pragma once
#include <string>
#include "Core/PollingStation.h"
namespace Dragonite
{
	class Object;
	class Component
	{
		friend Object;
	public:
		Component() = default;
		virtual ~Component() = default;

		virtual void Awake() = 0;
		virtual void Update(const float aDt) = 0;
		virtual void OnInspectorGUI() = 0;
		virtual std::string GetName() = 0;
	protected:
		Object* myObject;
		bool myActiveState;
		PollingStation* myPollingStation;
	};
}