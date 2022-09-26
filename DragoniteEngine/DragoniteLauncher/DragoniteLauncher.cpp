// DragoniteLauncher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <Core/Application.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <tchar.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int nCmdShow)
{
	Dragonite::Application* instance;

	Dragonite::ApplicationDesc desc;
	desc.myInitialResolution = { 1920, 1080 };
	desc.myWinClassName = L"Dragonite Engine";
	desc.myApplicationName = L"Test Application";
	desc.myNCmdShow = true;
	desc.myHInstance = hInstance;

	int result = 0;
	if (Dragonite::Application::CreateInstance(desc, &instance))
	{
		result = instance->ExecuteRuntime();
	}

	if (instance)
		delete instance;

	return result;
}



