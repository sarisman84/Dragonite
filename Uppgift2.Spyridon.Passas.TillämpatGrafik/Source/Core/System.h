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
		//System();
		//~System();
		bool Initialize(HINSTANCE anHInstance, int nCmdShow);
		void Shutdown();
		MSG StartRuntime();
		inline Graphics::GraphicsEngine* GetGraphicsEngine() { return &myGraphicsEngine; }
		inline WindowInfo& GetWindowsInfo() { return myWindowsInfo; }
	private:
		Graphics::GraphicsEngine myGraphicsEngine;
		WindowInfo myWindowsInfo;
		SystemState myRuntimeState;
	};
}



