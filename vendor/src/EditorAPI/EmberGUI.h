#pragma once
#include <wtypes.h>
#include "Space/WindowSpace.h"

#include <utility>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

struct ImGuiContext;

struct EmVec2
{
	EmVec2() = default;
	EmVec2(float anX, float anY) : x(anX), y(anY) {};
	float x, y;
};

namespace ember 
{
	class EmberWindow;
}


class EmberGUI
{
public:
	EmberGUI() = default;
	virtual const bool Init(HWND anInstance,
		ID3D11Device* aDevice,
		ID3D11DeviceContext* aContext,
		IDXGISwapChain* aSwapChain,
		ID3D11RenderTargetView* aViewToRenderTo) = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
	virtual LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual ImGuiContext* GetIMGUIContext() = 0;
	virtual EmVec2 GetViewportResolution() = 0;
	virtual void AddEditor(ember::EmberWindow* aNewWindow) = 0;
	virtual ID3D11Device*& GetDevice() = 0;
	virtual ID3D11DeviceContext*& GetContext() = 0;
	virtual ID3D11RenderTargetView*& GetBackBuffer() = 0;

#if CHOOSE_DESTRUCTION_TYPE
protected:
	~EmberGUI() {}
#else
	virtual ~EmberGUI() = default;
#endif

private:
	inline EmberGUI& operator=(const EmberGUI&) { return *this; }
	inline EmberGUI& operator=(EmberGUI&&) noexcept { return *this; }
};


