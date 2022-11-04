#pragma once
#include <wtypes.h>
#include "Space/WindowSpace.h"

#include <utility>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

struct ImGuiContext;

struct EmVec2
{
	EmVec2() = default;
	EmVec2(float anX, float anY) : x(anX), y(anY) {};
	float x, y;
};


class EmberGUI
{
	friend struct GUISpace;
public:
	EmberGUI() = default;
	virtual const bool Init(HWND anInstance, ID3D11Device* aDevice, ID3D11DeviceContext* aContext, IDXGISwapChain* aSwapChain) = 0;
	virtual void Update(const float aDt, ID3D11RenderTargetView* aTargetView) = 0;
	virtual void Shutdown() = 0;
	virtual LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual void* GetElements() = 0;
	virtual ImGuiContext* GetIMGUIContext() = 0;
	virtual EmVec2 GetViewportResolution() = 0;
	template<typename Event>
	void AddEditor(const char* aName, Event&& anEvent);
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
inline void EmberGUI::AddEditor(const char* aName, Event&& anEvent)
{
	auto editor = new EmberGUISpace(this, aName, std::forward<Event>(anEvent));
	this->AddSpace(editor);
}
