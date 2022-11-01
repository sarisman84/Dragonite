#pragma once
#include "EditorAPI/EmberGUI.h"

#include <wtypes.h>



struct APIInterface
{
	virtual bool Initialize(HWND& anInstance, EmberGUI* anEditorInterface = nullptr) = 0;
	virtual void Update(const float aDeltaTime) = 0;
	virtual LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;


#if CHOOSE_DESTRUCTION_TYPE
protected:
	~APIInterface() {};
#else
	virtual ~APIInterface() = default;
#endif

private:
	APIInterface& operator=(const APIInterface&);
	APIInterface& operator=(APIInterface&&);
};