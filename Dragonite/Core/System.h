#pragma once
#include "Graphics/GraphicsEngine.h"
#include "../EngineUtilities.h"
#include <map>
#include <typeinfo>
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
		~System();
		bool Initialize(HINSTANCE anHInstance, int nCmdShow);
		void Shutdown();
		MSG StartRuntime();
		inline Graphics::GraphicsEngine* GetGraphicsEngine() { return Get<Graphics::GraphicsEngine>(); }
		inline WindowInfo& GetWindowsInfo() { return myWindowsInfo; }
		
		template<class Manager>
		Manager* Get();

		template<class Manager>
		Manager* AddManager();
		
	private:

	

		Graphics::GraphicsEngine* myGraphicsEngine;
		WindowInfo myWindowsInfo;
		SystemState myRuntimeState;
		std::map<size_t, void*> myManagers;
	};


	template<class Manager>
	inline Manager* System::Get()
	{
		size_t hashKey = typeid(Manager).hash_code();
		for (auto& pair : myManagers)
		{
			if (hashKey == pair.first)
				return static_cast<Manager*>(pair.second);
		}
		return nullptr;
	}

	template<class Manager>
	inline Manager* System::AddManager()
	{
		size_t hashKey = typeid(Manager).hash_code();
		myManagers[hashKey] = static_cast<void*>(new Manager());
		return Get<Manager>();
	}
}



