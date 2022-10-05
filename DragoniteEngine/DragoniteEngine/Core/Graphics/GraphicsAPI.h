#pragma once
#include "DXIncludes.h"

#include "Core/CU/CommonData.h"
#include "Core/CU/Math/Matrix4x4.hpp"


#include <unordered_map>
#include <vector>
#include <memory>



namespace Dragonite
{
	class Camera;
	class Runtime;
	class ModelFactory;
	class TextureFactory;
	class ModelInstance;
	class BaseRenderer;
	class RenderFactory;
	class RenderTarget;

	enum class TextureSampleType
	{
		Default, Count
	};

	enum class BlendStateType
	{
		Alpha, Additive, None, Count
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
		friend class RenderTarget;
		friend class RenderInterface;
		friend class DragoniteGui;
		friend RenderFactory;
	public:
		GraphicsPipeline();
		~GraphicsPipeline();

		bool Initialize(Runtime* anApplication, HWND aWindowHandle);

		void SetBlendState(const BlendStateType aType);
		void SwitchRenderTarget(RenderView aView, DepthStencil aDepthBuffer = nullptr);

		void DrawInstructionsToBB();
		void DrawInstructions(RenderView aView, DepthStencil aDepthBuffer = nullptr);
		void Render();


		Vector2f GetViewPort();

		inline Runtime* GetApplication() { return myApplicationPtr; }
		inline Device& GetDevice() { return myDevice; }
		inline DeviceContext& GetContext() { return myContext; }



		inline void SetActiveCameraAs(Camera* aCamera) noexcept { myActiveCamera = aCamera; }
		inline std::vector<std::shared_ptr<ModelInstance>> GetInstructions() { return myElementsToDraw; };
		inline RenderView& GetBackBuffer() { return myBackBuffer; }
		inline DepthStencil& GetDefaultDepthBuffer() { return myDepthBuffer; }
		inline Color& ClearColor() { return myClearColor; }


		inline std::unordered_map<TextureSampleType, TextureSampler>& GetTextureSamplers() noexcept {
			return myTextureSamplers;
		}


		static HRESULT CreateBuffer(Device aDevice, DataBuffer& aBuffer, const DataBufferDesc& aDesc);
		static HRESULT CreateTexture(Device aDevice, DXTexture2D& aTexture, const TextureBufferDesc& aDesc);
		static HRESULT MapBuffer(DeviceContext aContext, DataBuffer& aBuffer, void* someData, size_t aDataSize);

		void UpdateFrameBuffer();
		void UpdateObjectBufferAt(std::shared_ptr<ModelInstance> anInstance);
		void UpdateBufferAt(void* someData, const size_t aSize, const int aSlot, DataBuffer& aBuffer, bool aBindVSFlag = true, bool aBindPSFlag = false);

	private:
		HRESULT InitializeBlendStates();
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
		RenderTarget* myActiveRenderTarget;

		std::vector<std::shared_ptr<ModelInstance>> myElementsToDraw;
		std::unordered_map<TextureSampleType, TextureSampler> myTextureSamplers;
		std::unordered_map<BlendStateType, BlendState> myBlendStates;
	};


	class RenderInterface
	{
	public:
		RenderInterface(GraphicsPipeline& aPipeline);

		void DrawElement(std::shared_ptr<ModelInstance> anInstance);

		inline void SetActiveCameraAs(Camera& aCameraRef) { myPipeline.SetActiveCameraAs(&aCameraRef); }

	private:
		GraphicsPipeline& myPipeline;
	};



}

