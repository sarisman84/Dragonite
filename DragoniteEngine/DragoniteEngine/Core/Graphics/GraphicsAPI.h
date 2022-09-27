#pragma once
#include "DXIncludes.h"
#include "../CU/CommonData.h"

#define DDLVISIBLE __declspec(dllexport)

namespace Dragonite
{
	namespace GraphicsAPI
	{
		



		class GraphicsPipeline
		{
		public:
			GraphicsPipeline();
			~GraphicsPipeline();

			bool Initialize(HWND aWindowHandle);
			void Render();

			inline Device& GetDevice() { return myDevice; }
			inline DeviceContext& GetContext() { return myContext; }

		private:
			HRESULT InitializeSwapChain(HWND anInstance);
			HRESULT InitializeBackBuffer();

			Device myDevice;
			DeviceContext myContext;
			SwapChain mySwapChain;
			TargetTexture myBackBuffer;
			Color myClearColor;


		};
	}


}

