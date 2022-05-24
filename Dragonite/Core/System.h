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

		inline const float GetTimeDelta() { return myTimeDelta; };
		inline const float GetTotalTime() { return myTotalTime; };

		template<class Manager>
		Manager* Get();

		template<class Manager, typename... Args>
		Manager* AddManager(Args&&... someArgs);


		inline const int GetFPS()
		{
			if (myTimeDelta == 0) return 0;
			auto result = static_cast<int>(myFrameCounter / myTotalTime);
			if (result > 2000)
				return 0;
			return result;
		}

	private:
		void ContainCursor();
		inline size_t GetManagerID();
		template<class Manager>
		inline size_t GetManagerTypeID();

		bool myLockMouseFlag;
		float myTimeDelta;
		float myTotalTime;
		int myFrameCounter;

		Graphics::GraphicsEngine* myGraphicsEngine;
		WindowInfo myWindowsInfo;
		SystemState myRuntimeState;
		std::unordered_map<size_t, void*> myManagers;
	};


	template<class Manager>
	inline Manager* System::Get()
	{
		size_t key = GetManagerTypeID<Manager>();

		for (auto& pair : myManagers)
		{
			if (key == pair.first)
				return static_cast<Manager*>(pair.second);
		}
		return nullptr;
	}

	template<class Manager, typename... Args>
	inline Manager* System::AddManager(Args&&... someArgs)
	{
		size_t key = GetManagerTypeID<Manager>();
		myManagers[key] = static_cast<void*>(new Manager(someArgs...));
		return Get<Manager>();
	}



	inline size_t Engine::System::GetManagerID()
	{
		static size_t counter;
		return counter++;
	}

	template<class Manager>
	inline size_t Engine::System::GetManagerTypeID()
	{
		static size_t key = GetManagerID();
		return key;
	}
}



