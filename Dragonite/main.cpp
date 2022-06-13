#include "Core/System.h"





int APIENTRY wWinMain(_In_ HINSTANCE anHInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	Dragonite::System system;
	if (!system.Initialize(anHInstance, nCmdShow))
	{
		return 0;
	}

	return (int)system.StartRuntime().wParam;
}
