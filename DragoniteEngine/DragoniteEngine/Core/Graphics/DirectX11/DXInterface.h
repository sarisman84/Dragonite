#pragma once
#include "Core/CU/CommonData.h"
#include "DXIncludes.h"
#include <unordered_map>

struct D3D11_TEXTURE2D_DESC;
struct D3D11_BLEND_DESC;
namespace Dragonite
{
#define VIEWPORT_MAIN 0
#define BLEND_NONE 0
#define BLEND_ALPHA 1
#define BLEND_ADDITIVE 2

	struct Data;
	struct BufferDesc;
	struct TextureDesc;
	struct SamplerDesc;
	struct Viewport;


	class DXInterface
	{
	public:
		static Device Device;
		static DeviceContext Context;
		static SwapChain SwapChain;
		static int VSyncState;
		static Color ClearColor;
		static bool Fullscreen;

		static const bool Init(HWND anInstance);
		static void Present();
		static const bool RefreshRender();

		static const HRESULT InitializeSwapChain(HWND anInstance = nullptr);

		static const HRESULT CreateSampler(const unsigned int aKey, SamplerDesc* aSamplerDesc);
		static const HRESULT CreateViewPort(const unsigned int aKey, Viewport* aViewport);
		static const HRESULT CreateBlendState(const unsigned int aKey, D3D11_BLEND_DESC* aDesc);
		static const HRESULT CreateBuffer(DataBuffer& aBuffer, BufferDesc* someBufferDesc);
		static const HRESULT CreateTexture(DXTexture2D& aTexture, D3D11_TEXTURE2D_DESC* someTextureDesc);
		static const HRESULT CreateDepthStencil(const DXTexture2D& aTexture, DepthStencil& aDepthStencil);
		static const HRESULT CreateVSInstance(const char* aPath, VertexShader& aShader, std::string& someExtraData);
		static const HRESULT CreatePSInstance(const char* aPath, PixelShader& aShader);

		static const HRESULT ModifyBuffer(DataBuffer& aBuffer, Data&& someData);
		static const HRESULT SetViewport(const unsigned int aKey = VIEWPORT_MAIN);
		static const Vector2f GetViewportResolution(const unsigned int aKey = VIEWPORT_MAIN);
		static TextureSampler GetSampler(unsigned int aKey = 0);
		static DepthStencil GetDepthBuffer() { return myInstance.myDepthBuffer; }
		static RenderView GetBackBuffer() { return myInstance.myBackBuffer; }

		static void SetBlendState(unsigned int aNewState);
		static void DrawTo(RenderView aTarget, DepthStencil aDepthStencil = nullptr);
		static void DrawToBackBuffer();
		static void ClearTarget(RenderView aTarget, DepthStencil aDepthStencil = nullptr);
		static void SwitchRenderTarget(RenderView aTarget, DepthStencil aDepthStencil = nullptr);
	private:
		static DXInterface myInstance;
		const HRESULT InitializeBackBuffer();


		HWND myWindowInstance;

		RenderView myBackBuffer;
		DepthStencil myDepthBuffer;


		std::unordered_map<unsigned int, BlendState> myBlendStates;
		std::unordered_map<unsigned int, TextureSampler> mySamplers;
		std::unordered_map<unsigned int, Viewport*> myViewports;
	};







}


