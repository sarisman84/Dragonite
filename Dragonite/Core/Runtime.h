#pragma once

namespace Engine
{
	class System;

	class Runtime
	{
		friend System;
	private:
		Runtime(System* aCoreSystem);
	protected:
		System* mySystem;
		virtual void Awake();
		virtual void Update();
	};
}


