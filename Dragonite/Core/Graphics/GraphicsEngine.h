#pragma once
#include <wrl/client.h>
#include <vector>
#include <memory>

#include "Graphics/Rendering/Render.h"
#include "../../EngineUtilities.h"


struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;



using Microsoft::WRL::ComPtr;
using Engine::Windows::Resolution;

namespace Engine
{
	namespace Graphics
	{
		class GraphicsEngine
		{
		public:
			GraphicsEngine();
			~GraphicsEngine();
			bool Initialize(Engine::Windows::Resolution aResolution, HWND aWindowsHandle);
			void DrawElements();

			template<class Element, typename... Args>
			std::shared_ptr<Element> CreateElement(Args&&... someArgs);
		private:
			ComPtr<ID3D11Device> myDevice;
			ComPtr<ID3D11DeviceContext> myContext;
			ComPtr<IDXGISwapChain> mySwapChain;
			ComPtr<ID3D11RenderTargetView> myBackBuffer;


			std::vector<std::shared_ptr<Render>> myRenderTargets;
		};



		template<class Element, typename ...Args>
		inline std::shared_ptr<Element> GraphicsEngine::CreateElement(Args&&... someArgs)
		{
			std::shared_ptr<Render> element = std::make_shared<Element>(myDevice.Get(), myContext.Get(), someArgs...);
			if (!element->Initialize()) return nullptr;
			myRenderTargets.push_back(element);
			return std::dynamic_pointer_cast<Element>(myRenderTargets.back());
		}
	}

}


