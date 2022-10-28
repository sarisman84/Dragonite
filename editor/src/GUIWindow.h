#pragma once
#include "EditorSettings.h"
//#include "Core/PollingStation.h"
//#include "Core/External/imgui/imgui.h"
//#include "Core/External/imgui/ImGuizmo.h"
//#include "Core/PollingStation.h"

#include "imgui.h"
#include "ImGuizmo.h"

namespace Ember
{
	class AbsWindows
	{
	public:
		virtual void Update() = 0;
		inline const bool IsBeingHovered() const { return myHoveredFlag; };
		inline const bool IsBeingFocused() const { return myFocusedFlag; };
		inline const unsigned int GetUUID() const { return myID; }
	protected:
		bool myHoveredFlag, myFocusedFlag;
		unsigned int myID;
	};



	template<typename Render>
	class Window : public AbsWindows
	{
	public:
		Window(Render&& aRenderCall);
		void Update() override;


	private:
		Render myRenderCall;



	};

	template<typename Render>
	inline Window<Render>::Window(Render&& aRenderCall)
	{
		static unsigned int nextID;
		myID = nextID++;

		myRenderCall = aRenderCall;
	}
	template<typename Render>
	inline void Window<Render>::Update()
	{
		myHoveredFlag = ImGui::IsWindowHovered();
		myFocusedFlag = ImGui::IsWindowFocused();

		myRenderCall(this);
	}
}


