#pragma once
#include <wrl/client.h>
#include "../../EngineUtilities.h"


using Microsoft::WRL::ComPtr;
using Engine::Windows::Resolution;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace Engine
{
	namespace Graphics
	{
		class GraphicsEngine
		{
		public:
			GraphicsEngine() = default;
			~GraphicsEngine() = default;
			bool Initialize(Engine::Windows::Resolution aResolution, HWND aWindowsHandle);
			void Render();
		private:
			ComPtr<ID3D11Device> myDevice;
			ComPtr<ID3D11DeviceContext> myContext;
			ComPtr<IDXGISwapChain> mySwapChain;
			ComPtr<ID3D11RenderTargetView> myBackBuffer;
		};
	}
	
}


