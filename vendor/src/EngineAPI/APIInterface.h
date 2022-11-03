#pragma once
#include "EditorAPI/EmberGUI.h"

#include <wtypes.h>



struct EngineAPI
{
	virtual bool Initialize(HWND& anInstance, EmberGUI* anEditorInterface = nullptr) = 0;
	virtual void Update(const float aDeltaTime) = 0;
	virtual LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;


#if CHOOSE_DESTRUCTION_TYPE
protected:
	~APIInterface() {};
#else
	virtual ~EngineAPI() = default;
#endif

private:
	EngineAPI& operator=(const EngineAPI&);
	EngineAPI& operator=(EngineAPI&&);
};
