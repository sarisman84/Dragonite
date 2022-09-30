#include "Input.h"

#include "Core/Application.h"
#include "Core/Graphics/GraphicsAPI.h"

Dragonite::Mouse::Mouse() = default;

const bool Dragonite::Mouse::Init(Application* anApplication)
{
	myApplicationIns = anApplication;
	myGraphicsPipeline = anApplication->GetPollingStation().Get<GraphicsPipeline>();

	myApplicationIns->WndProcs() += [this](HWND, UINT aMessage, WPARAM aWParam, LPARAM anLParam)
	{
		Update(aMessage, aWParam, anLParam);
	};

	myApplicationIns->UpdateCallbacks() += [this](const float aDt)
	{
		EndFrame();
	};

	return true;
}

const bool Dragonite::Mouse::GetButton(MouseKey aKey)
{
	int key = static_cast<int>(aKey);
	return myState[key];
}

const bool Dragonite::Mouse::GetButtonDown(MouseKey aKey)
{
	int key = static_cast<int>(aKey);
	bool result = myState[key] && !myPreviousState[key];
	myPreviousState[key] = myState[key];
	return result;
}

const bool Dragonite::Mouse::GetButtonUp(MouseKey aKey)
{
	int key = static_cast<int>(aKey);
	bool result = !myState[key] && myPreviousState[key];
	myPreviousState[key] = myState[key];
	return result;
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
		position.x = point.x;
		position.y = point.y;
		break;
	default:
		break;
	}


	if (IsAMouseButtonEvent(aMessage))
	{
		bool state = myState[aMessage];
		switch (aMessage)
		{

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			state = true;
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			state = false;
			break;


		default:
			break;
		}
		myState[aMessage] = state;
	}



}

const bool Dragonite::InputManager::Init(Application* anApplication)
{
	if (!myMouse.Init(anApplication))
		return false;
	return true;
}
