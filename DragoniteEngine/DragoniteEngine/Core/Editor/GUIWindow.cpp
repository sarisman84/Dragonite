#include "GUIWindow.h"

Dragonite::GUIWindow::GUIWindow(const char* aWindowName) : myWindowName(aWindowName), myActiveStateFlag(true)
{
}

void Dragonite::GUIWindow::UpdateWindowState()
{
	if (myActiveStateFlag)
		OnEnable();
	else
		OnDisable();
}


