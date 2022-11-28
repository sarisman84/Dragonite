#pragma once
#include "Core/CU/Math/Vector2.h"

#define NOMINMAX
#include <Windows.h>
#include <bitset>


#pragma warning (disable: 4244)

namespace Dragonite
{
	class Engine;
	class GraphicalInterface;

	enum class MouseKey : unsigned int
	{
		Left = WM_LBUTTONDOWN | WM_LBUTTONUP,
		Right = WM_RBUTTONDOWN | WM_RBUTTONDOWN,
		Middle = WM_MBUTTONDOWN | WM_MBUTTONUP
	};



	class Mouse
	{
	public:
		Mouse();
		const bool Init(Engine* anApplication);

		const bool GetButton(MouseKey aKey);
		const bool GetButtonDown(MouseKey aKey);
		const bool GetButtonUp(MouseKey aKey);


		void ViewMouse(const bool aState);

		void ResetPos();

		void WinProc(HWND, UINT, WPARAM, LPARAM);


		inline void SetOffset(const Vector2f& anOffset)
		{
			myOffset = anOffset;
		}

		Vector2f position;
		Vector2f absPosition;
		Vector2f delta;

	private:
		void EndFrame();
		void Update(UINT aMessage, WPARAM aWParam, LPARAM anLParam);

		void UpdateState(UINT aButton);

		Vector2f myPreviousPos;
		Vector2f myOffset;


		std::bitset<3> myDownState;
		std::bitset<3> myPreviousDownState;

		std::bitset<3> myUpState;
		std::bitset<3> myPreviousUpState;

		Engine* myApplicationIns;

	};





	class InputManager
	{
	public:
		InputManager() = default;



		const bool Init(Engine* anApplication);

		inline Mouse& GetMouse() { return myMouse; }
	private:
		Mouse myMouse;
	};

}