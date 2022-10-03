#pragma once
#include "DXIncludes.h"

#include "Core/CU/CommonData.h"
#include "Core/CU/Math/Matrix4x4.hpp"

#include <unordered_map>
#include <vector>
#include <memory>

#define DDLVISIBLE __declspec(dllexport)

namespace Dragonite
{
	class Camera;
	class Runtime;
	class ModelFactory;
	class TextureFactory;
	class ModelInstance;
	class BaseRenderer;
	class  RenderFactory;

	enum class TextureSampleType
	{
		Default
	};

	struct FrameBufferData
	{
		Matrix4x4f myWorldToClipMatrix;
	};

	struct ObjectBufferData
	{
		Matrix4x4f myModelToWorldMatrix;
	};




	class GraphicsPipeline
	{
		friend class RenderInterface;
		friend class DragoniteGui;
		friend RenderFactory;
	public:
		GraphicsPipeline();
		~GraphicsPipeline();

		bool Initialize(Runtime* anApplication, HWND aWindowHandle);
		void Render();

		inline Runtime* GetApplication() { return myApplicationPtr; }
		inline Device& GetDevice() { return myDevice; }
		inline DeviceContext& GetContext() { return myContext; }

		inline void SetActiveCameraAs(Camera* aCamera) noexcept { myActiveCamera = aCamera; }


		void DrawToNewRenderTarget(const RenderView& aTarget, const RasterizerState& aNewState = nullptr);
		void DrawToBackBuffer();

		inline std::unordered_map<TextureSampleType, TextureSampler>& GetTextureSamplers() noexcept {
			return myTextureSamplers;
		}


		static HRESULT CreateBuffer(Device aDevice, DataBuffer& aBuffer, const DataBufferDesc& aDesc);
		static HRESULT CreateTexture(Device aDevice, DXTexture2D& aTexture, const TextureBufferDesc& aDesc);
		static HRESULT SetBuffer(DeviceContext aContext, DataBuffer& aBuffer, void* someData, size_t aDataSize);

	private:
		HRESULT InitializeSwapChain(HWND anInstance);
		HRESULT InitializeBackBuffer();
		HRESULT InitializeSamplers();

		Device myDevice;
		DeviceContext myContext;
		SwapChain mySwapChain;
		RenderView myBackBuffer;
		DepthStencil myDepthBuffer;
		Color myClearColor;

		DataBuffer myFrameBuffer;
		DataBuffer myObjectBuffer;

		RenderFactory* myRenderFactory;
		Runtime* myApplicationPtr;
		ModelFactory* myModelFactory;
		TextureFactory* myTextureFactory;
		Camera* myActiveCamera;

		std::vector<std::unique_ptr<BaseRenderer>> myRenderers;
		std::vector<std::shared_ptr<ModelInstance>> myElementsToDraw;
		std::unordered_map<TextureSampleType, TextureSampler> myTextureSamplers;
	};


	class RenderInterface
	{
	public:
		RenderInterface(GraphicsPipeline& aPipeline);

		DDLVISIBLE void DrawElement(std::shared_ptr<ModelInstance> anInstance);

		DDLVISIBLE inline void SetActiveCameraAs(Camera& aCameraRef) { myPipeline.SetActiveCameraAs(&aCameraRef); }

	private:
		GraphicsPipeline& myPipeline;
	};



}

