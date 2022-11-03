#pragma once
#include <wtypes.h>

namespace Dragonite
{
	struct IWinEvent
	{
		IWinEvent() { static unsigned int id = 0; myID = id++; }
		virtual LRESULT Invoke(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

		const bool operator==(IWinEvent* anOtherEvent) const { return anOtherEvent && myID == anOtherEvent->myID; }
	private:
		unsigned int myID;
	};
	template<typename Event>
	struct WindowsEvent : public IWinEvent
	{
		WindowsEvent() = default;
		WindowsEvent(Event&& anEvent) : IWinEvent(), myEvent(anEvent) {}
		// Inherited via IWinEvent
		LRESULT Invoke(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			return myEvent(hWnd, message, wParam, lParam);
		}

	private:
		Event myEvent;
	};


	struct IUpdateEvent
	{
		virtual void Invoke(const float aDt) = 0;
	};
	template<typename Event>
	struct UpdateEvent : public IUpdateEvent
	{
		UpdateEvent(Event&& anEvent) : IUpdateEvent(), myEvent(anEvent) {}

		void Invoke(const float aDt) override {
			myEvent(aDt);
		}
	private:
		Event myEvent;
	};


#pragma region Test Stuff
	struct IEvent
	{
	};


	template<typename, typename Call>
	struct Event : public IEvent
	{
	};


	template<typename Call, typename Ret, typename... Args>
	struct Event<Ret(Args...), Call> : public IEvent
	{
		Ret operator()(Args... someArgs)
		{
			return myCall(someArgs...);
		}

	private:
		Call myCall;
	};

#pragma endregion


}