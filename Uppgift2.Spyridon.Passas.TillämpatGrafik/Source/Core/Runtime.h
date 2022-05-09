#pragma once
#include "Rendering/RenderObject.h"
namespace Engine
{
	class System;

	class Runtime
	{
		friend System;
	private:
		Runtime(System* aCoreSystem);
		int someCount;
		Object someShape;
		Object someOtherShape;
	protected:
		System* mySystem;
		virtual void Awake();
		virtual void Update();
	};
}


