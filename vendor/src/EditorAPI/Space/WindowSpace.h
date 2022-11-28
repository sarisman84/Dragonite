#pragma once
#include <vector>
#include <memory>
#include "imgui/imgui.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;


class EmberGUI;

namespace ember
{
	class EmberWindow
	{
	public:
		EmberWindow(const char* aName) : myName(aName) {};
		virtual ~EmberWindow();
		virtual void OnInit() {}
		virtual void OnEarlyGUI() {}
		virtual void OnGUI() = 0;
		virtual void OnLateGUI() {};
	public:
		void Render();

		inline void Init(EmberGUI* anInterface) { myInterface = anInterface; OnInit(); }
		void PrepareIMGUI();
		void RenderChildContexts();
		inline void AddChildWindow(EmberWindow* aNewWindow)
		{
			aNewWindow->myInterface = myInterface;
			myChildContexts.push_back(std::shared_ptr<EmberWindow>(aNewWindow));
		}

		ID3D11Device*& DXDevice();
		ID3D11DeviceContext*& DXContext();
		ID3D11RenderTargetView*& BackBuffer();

	protected:
		const char* myName;

	private:
		std::vector<std::shared_ptr<EmberWindow>> myChildContexts;
		EmberGUI* myInterface;
	};
}
