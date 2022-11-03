#pragma once
#include <wtypes.h>
#include "Space/WindowSpace.h"

#include <utility>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

class EmberGUI
{
public:
	EmberGUI() = default;
	virtual const bool Init(HWND anInstance, ID3D11Device* aDevice, ID3D11DeviceContext* aContext, IDXGISwapChain* aSwapChain) = 0;
	virtual void Update(const float aDt, ID3D11RenderTargetView* aTargetView) = 0;
	virtual void Shutdown() = 0;
	virtual LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	template<typename Event>
	void AddSpace(Event&& anEvent);
	virtual void AddSpace(GUISpace* aNewSpace) = 0;

#if CHOOSE_DESTRUCTION_TYPE
protected:
	~EmberGUI() {}
#else
	virtual ~EmberGUI() = default;
#endif

private:
	virtual ID3D11Device* GetDevice() = 0;
	virtual ID3D11DeviceContext* GetContext() = 0;

	inline EmberGUI& operator=(const EmberGUI&) { return *this; }
	inline EmberGUI& operator=(EmberGUI&&) noexcept { return *this; }
};



template<typename Event>
inline void EmberGUI::AddSpace(Event&& anEvent)
{
	AddSpace(new EmberGUISpace(GetDevice(), GetContext(), std::forward<Event>(anEvent)));
}
