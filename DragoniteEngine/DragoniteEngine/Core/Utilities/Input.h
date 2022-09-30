#pragma once
#include "Core/CU/Math/Vector2.h"

#define NOMINMAX
#include <Windows.h>
#include <bitset>
namespace Dragonite
{
	class Application;
	class GraphicsPipeline;

	enum class MouseKey : unsigned int
	{
		Left = WM_LBUTTONDBLCLK | WM_LBUTTONDOWN | WM_LBUTTONUP, 
		Right = WM_RBUTTONDBLCLK | WM_RBUTTONDOWN | WM_RBUTTONUP, 
		Middle = WM_MBUTTONDBLCLK | WM_MBUTTONDOWN | WM_MBUTTONUP
	};



	class Mouse
	{
	public:
		Mouse();
		const bool Init(Application* anApplication);

		const bool GetButton(MouseKey aKey);
		const bool GetButtonDown(MouseKey aKey);
		const bool GetButtonUp(MouseKey aKey);

		Vector2f position;
		Vector2f absPosition;
		Vector2f delta;

	private:
		void EndFrame();
		void Update(UINT aMessage, WPARAM aWParam, LPARAM anLParam);

		Vector2f myPreviousPos;

		std::bitset<1000> myState;
		std::bitset<1000> myPreviousState;


		Application* myApplicationIns;
		GraphicsPipeline* myGraphicsPipeline;

	};





	class InputManager {
	public:
		InputManager() = default;



		const bool Init(Application* anApplication);

		inline Mouse& GetMouse() { return myMouse; }
	private:
		Mouse myMouse;
	};

}