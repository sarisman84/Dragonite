#include "Input.h"

#include "Pipeline/Runtime.h"
#include "Core/Graphics/GraphicsAPI.h"
#include "Core/PollingStation.h"

Dragonite::Mouse::Mouse() = default;

const bool Dragonite::Mouse::Init(Engine* anApplication)
{
	myApplicationIns = anApplication;
	myGraphicsPipeline = anApplication->GetPollingStation().Get<GraphicalInterface>();

	myApplicationIns->RegisterWinProcListener([this](HWND, UINT aMessage, WPARAM aWParam, LPARAM anLParam) { Update(aMessage, aWParam, anLParam); });

	myApplicationIns->RegisterUpdateListener([this](const float aDT) { EndFrame(); });

	return true;
}

const bool Dragonite::Mouse::GetButton(MouseKey aKey)
{
	int key = static_cast<int>(aKey);
	UINT button = (key - WM_LBUTTONDOWN) / 3;
	bool r = myDownState[button];
	return r;
}

const bool Dragonite::Mouse::GetButtonDown(MouseKey aKey)
{
	int key = static_cast<int>(aKey);
	UINT button = (key - WM_LBUTTONDOWN) / 3;
	bool result = myDownState[button] && !myPreviousDownState[button];
	myPreviousDownState[button] = myDownState[button];
	return result;
}

const bool Dragonite::Mouse::GetButtonUp(MouseKey aKey)
{
	int key = static_cast<int>(aKey);
	UINT button = (key - WM_LBUTTONDOWN) / 3;
	bool result = !myUpState[button] && myPreviousUpState[button];
	myPreviousUpState[button] = myUpState[button];
	return result;
}

void Dragonite::Mouse::ViewMouse(const bool aState)
{
	if (aState)
		while (ShowCursor(true) < 1);
	else
		while (ShowCursor(false) > 0);
}

void Dragonite::Mouse::ResetPos()
{
	RECT aRect;
	if (GetClientRect(myApplicationIns->GetClientInstance(), &aRect))
	{
		float width = aRect.right - aRect.left;
		float height = aRect.bottom - aRect.top;
		myPreviousPos = { width / 2.0f ,height / 2.0f };
		position = { width / 2.0f ,height / 2.0f };
	}

	if (GetWindowRect(myApplicationIns->GetClientInstance(), &aRect))
	{
		float width = aRect.right - aRect.left;
		float height = aRect.bottom - aRect.top;
		absPosition = { width / 2.0f ,height / 2.0f };

		SetCursorPos(width / 2.0f, height / 2.0f);
	}
	delta = { 0,0 };



}



void Dragonite::Mouse::EndFrame()
{
	delta = { 0,0 };
	delta = position - myPreviousPos;
	myPreviousPos = position;
}


const bool IsAMouseButtonEvent(const UINT aMessage)
{
	return
		aMessage == WM_LBUTTONDOWN ||
		aMessage == WM_LBUTTONUP ||
		aMessage == WM_LBUTTONDBLCLK ||

		aMessage == WM_RBUTTONDOWN ||
		aMessage == WM_RBUTTONUP ||
		aMessage == WM_RBUTTONDBLCLK ||

		aMessage == WM_MBUTTONDOWN ||
		aMessage == WM_MBUTTONUP ||
		aMessage == WM_MBUTTONDBLCLK;
}




void Dragonite::Mouse::Update(UINT aMessage, WPARAM aWParam, LPARAM anLParam)
{




	switch (aMessage)
	{
	case WM_MOUSEMOVE:
		auto point = MAKEPOINTS(anLParam);
		absPosition.x = point.x;
		absPosition.y = point.y;

		position = absPosition - myOffset;
		break;
	default:
		break;
	}


	if (IsAMouseButtonEvent(aMessage))
	{
		UpdateState(aMessage);
	}



}

void Dragonite::Mouse::UpdateState(UINT aButton)
{
	UINT buttonEvent = (aButton - WM_LBUTTONDOWN) / 3;

	myDownState[buttonEvent] = aButton == WM_LBUTTONDOWN || aButton == WM_RBUTTONDOWN || aButton == WM_MBUTTONDOWN;
	myUpState[buttonEvent] = aButton == WM_LBUTTONUP || aButton == WM_RBUTTONUP || aButton == WM_MBUTTONUP;
}

const bool Dragonite::InputManager::Init(Engine* anApplication)
{
	if (!myMouse.Init(anApplication))
		return false;
	return true;
}
