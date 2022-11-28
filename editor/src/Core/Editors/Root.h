#pragma once
#include "EditorAPI/Space/WindowSpace.h"

class DropArea : public ember::EmberWindow
{
public:
	DropArea();
	void OnEarlyGUI() override;
	void OnGUI() override;
	void OnLateGUI() override;
private:
	void BeginDockingSpace();
	void EndDockingSpace();
};

