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
	namespace GraphicsAPI
	{
		class GraphicsPipeline;
	}

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
		~RuntimeHandler();

		RuntimeHandler(const Application& anOtherIns) = delete;
		void operator=(const Application& anOtherIns) = delete;


		template<typename Type>
		inline Type* Get()
		{
			if (myUniqueDataMembers.count(typeid(Type).hash_code()) > 0)
				return (Type*)myUniqueDataMembers[typeid(Type).hash_code()];
			return nullptr;
		}

	private:
		template<typename Type>
		Type* AddHandler(Type* anInstance)
		{
			myUniqueDataMembers[typeid(Type).hash_code()] = anInstance;
			return anInstance;
		}

		std::unordered_map<size_t, void*> myUniqueDataMembers;
	};



	class Application
	{
	public:
		static DDLVISIBLE const bool CreateInstance(const ApplicationDesc& anApplicationDesc, Application** anOutput);

		DDLVISIBLE Application();
		DDLVISIBLE ~Application();

		int DDLVISIBLE ExecuteRuntime();
		inline DDLVISIBLE RuntimeHandler& GetInstance() { return myRuntimeHandler; }
		DDLVISIBLE LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		bool myRuntimeState;

		HWND myInstance;
		RuntimeHandler myRuntimeHandler;
		GraphicsAPI::GraphicsPipeline* myPipeline;
	};


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


}

