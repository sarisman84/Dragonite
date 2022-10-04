#pragma once
#include "Core/CU/Math/Vector2.h"

#define NOMINMAX
#include <Windows.h>
#include <bitset>


#pragma warning (disable: 4244)

namespace Dragonite
{
	class Runtime;
	class GraphicsPipeline;

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
		const bool Init(Runtime* anApplication);

		const bool GetButton(MouseKey aKey);
		const bool GetButtonDown(MouseKey aKey);
		const bool GetButtonUp(MouseKey aKey);


		void ViewMouse(const bool aState);

		void ResetPos();

		Vector2f position;
		Vector2f absPosition;
		Vector2f delta;

	private:
		void EndFrame();
		void Update(UINT aMessage, WPARAM aWParam, LPARAM anLParam);

		void UpdateState(UINT aButton);

		Vector2f myPreviousPos;


		
		std::bitset<3> myDownState;
		std::bitset<3> myPreviousDownState;

		std::bitset<3> myUpState;
		std::bitset<3> myPreviousUpState;

		Runtime* myApplicationIns;
		GraphicsPipeline* myGraphicsPipeline;

	};





	class InputManager
	{
	public:
		InputManager() = default;



		const bool Init(Runtime* anApplication);

		inline Mouse& GetMouse() { return myMouse; }
	private:
		Mouse myMouse;
	};

}