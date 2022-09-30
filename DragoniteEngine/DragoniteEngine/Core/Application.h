#pragma once
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include <typeinfo>

#include <unordered_map>

#include "Utilities/Function.h"
#include "CU/CommonData.h"
#include <chrono>

#define DDLVISIBLE __declspec(dllexport)



namespace Dragonite
{

	using Time = std::chrono::time_point<std::chrono::steady_clock>;
	using Clock = std::chrono::high_resolution_clock;

	class GraphicsPipeline;


	struct ApplicationDesc
	{
		_In_ HINSTANCE		myHInstance;
		_In_opt_ HINSTANCE  myHPrevInstance;
		_In_ LPSTR			myLPCmdLine;
		_In_ int			myNCmdShow = 0;
		LPCWSTR				myWinClassName = L"Dragonite Engine";
		Resolution			myInitialResolution = { 1920, 1080 };
		LPCWSTR				myApplicationName;
	};



	class RuntimeHandler
	{
		friend GraphicsPipeline;
		friend class Application;
	public:
		RuntimeHandler() = default;
		~RuntimeHandler();

		RuntimeHandler(const Application& anOtherIns) = delete;
		void operator=(const Application& anOtherIns) = delete;


		template<typename Type>
		inline Type* Get()
		{
			if (myUniqueDataMembers.count(typeid(Type).hash_code()) > 0)
				return std::static_pointer_cast<Type>(myUniqueDataMembers[typeid(Type).hash_code()]).get();
			return nullptr;
		}

	private:
		template<typename Type>
		Type* AddHandler(Type* anInstance)
		{
			myUniqueDataMembers[typeid(Type).hash_code()] = std::shared_ptr<Type>(anInstance);
			return anInstance;
		}

		std::unordered_map<size_t, std::shared_ptr<void>> myUniqueDataMembers;
	};



	class Application
	{
	public:
		static DDLVISIBLE const bool CreateInstance(const ApplicationDesc& anApplicationDesc, Application** anOutput);

		DDLVISIBLE Application();
		DDLVISIBLE ~Application();

		int DDLVISIBLE ExecuteRuntime();
		inline DDLVISIBLE RuntimeHandler& GetPollingStation() { return myRuntimeHandler; }
		DDLVISIBLE LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		inline Function<void(HWND, UINT, WPARAM, LPARAM)>& WndProcs() { return myWndProcs; }
		inline Function<void(const float)>& UpdateCallbacks() { return myUpdateCB; }
	private:
		bool myRuntimeState;
		HWND myInstance;
		RuntimeHandler myRuntimeHandler;
		GraphicsPipeline* myPipeline;

		Function<void(const float)> myUpdateCB;
		Function<void(HWND, UINT, WPARAM, LPARAM)> myWndProcs;


	};


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


}

