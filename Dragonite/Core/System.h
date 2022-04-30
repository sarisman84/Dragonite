#pragma once
#include "Graphics/GraphicsEngine.h"
#include "../EngineUtilities.h"
using namespace Engine::Windows;
namespace Engine
{
	enum class SystemState
	{
		Run, Exit
	};

	class System
	{
	public:
		System() = default;
		bool Initialize(HINSTANCE anHInstance, int nCmdShow);
		void Shutdown();
		MSG StartRuntime();
	private:
		Graphics::GraphicsEngine myGraphicsEngine;
		WindowInfo myWindowsInfo;
		SystemState myRuntimeState;
		void EngineRuntime();
	};
}



