#pragma once
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include <typeinfo>

#include <unordered_map>

#include "CU/CommonData.h"


#define DDLVISIBLE __declspec(dllexport)

namespace Dragonite
{

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
		friend class Application;
	public:
		RuntimeHandler() = default;
		~RuntimeHandler() = default;

		RuntimeHandler(const Application& anOtherIns) = delete;
		void operator=(const Application& anOtherIns) = delete;


		template<typename Type>
		inline Type* Get()
		{
			if (myUniqueDataMembers.count(&typeid(Type)) > 0)
				return (Type*)myUniqueDataMembers[&typeid(Type)];
			return nullptr;
		}

	private:
		template<typename Type>
		void AddHandler(Type* anInstance)
		{
			myUniqueDataMembers[&typeid(Type)] = anInstance;
		}

		std::unordered_map<std::type_info*, void*> myUniqueDataMembers;
	};



	class Application
	{
	public:
		static DDLVISIBLE const bool CreateInstance(const ApplicationDesc& anApplicationDesc, Application** anOutput);
		int DDLVISIBLE ExecuteRuntime();
		inline DDLVISIBLE RuntimeHandler& GetInstance() { return myRuntimeHandler; }
		DDLVISIBLE LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		bool myRuntimeState;

		HWND myInstance;
		RuntimeHandler myRuntimeHandler;
	};


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


}

