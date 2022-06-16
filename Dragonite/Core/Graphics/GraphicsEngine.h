#pragma once
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <string>
#include <queue>

#include "../../EngineUtilities.h"
#include "Utilities/Math/Matrix.h"
#include "Utilities/Math/Vector.h"

#include "Utilities/Algorithms/SparseSet.h"

#pragma warning (disable: 4471)

using Microsoft::WRL::ComPtr;
using Dragonite::Windows::Resolution;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;
struct ID3D11DepthStencilView;
struct ID3D11SamplerState;


enum D3D11_FILTER;
enum D3D11_TEXTURE_ADDRESS_MODE;


enum class Primitive2D;
class Camera;
struct ModelInstance;


struct FrameBufferData
{
	Math::Matrix4x4f myClipSpaceMatrix;
	float myTimeDelta;
	float myTotalTime;
	float myPadding[2];
	Math::Vector4f myCameraPosition;

	Math::Vector4f myLightDirection;
	Math::Vector4f myLightColor;
	Math::Vector4f myAmbientLight;
};

struct ObjectBufferData
{
	Math::Matrix4x4f myObjectMatrix;
	Math::Vector4f myColor;
	Math::Vector4f mySize;
};

struct StaticBufferData
{
	Math::Vector2f myResolution;
	Math::Vector2f myPadding;
};

struct LightInfo
{
	Math::Vector4f myLightDirection;
	Math::Vector4f myLightColor;
	Math::Vector4f myAmbientLight;
};

class RenderTarget;
namespace Dragonite
{
	class System;
	class Texture;
	


	class GraphicsEngine
	{
	public:
		GraphicsEngine();
		~GraphicsEngine();
		bool Initialize(Dragonite::Windows::Resolution aResolution, HWND aWindowsHandle, System* aSystem);

		inline void AddRenderInstruction(ModelInstance* anInstance)
		{
			myRenderInstructions.Insert(anInstance);
		}

		inline LightInfo& GlobalLightData()
		{
			return myLightData;
		}



		inline void SetRenderCamera(Camera* aCamera) { myRenderCamera = aCamera; }
		inline Camera* GetRenderCamera() noexcept { return myRenderCamera; }

		void DrawElements();
		inline ID3D11Device* GetDevice() { return myDevice.Get(); }
		inline ComPtr<ID3D11DeviceContext>& GetDeviceContext() { return myContext; }

		inline ComPtr<ID3D11Buffer>& ObjectBuffer() { return myObjectBuffer; }
		inline ComPtr<ID3D11Buffer>& FrameBuffer() { return myFrameBuffer; }
		inline void SetRenderTarget(RenderTarget* aTarget) { myRenderTarget = aTarget; }
		inline Dragonite::Windows::Resolution& Viewport() { return myViewport; }


		void UpdateConstantBuffer(ComPtr<ID3D11Buffer>& aConstantBuffer, void* someData, const size_t someDataSize, const UINT aSlot,
			void (ID3D11DeviceContext::* anOnConstantBufferUpdateCallback)(UINT aStartSlot, UINT aNumBuffers, ID3D11Buffer* const* aConstantBuffer),
			void (ID3D11DeviceContext::* anotherOnConstantBufferUpdateCallback)(UINT aStartSlot, UINT aNumBuffers, ID3D11Buffer* const* aConstantBuffer) = nullptr);

		/*	void std::shared_ptr<RenderObject> CreateElement(Primitive aPrimitiveShape);*/

		//std::shared_ptr<Mesh> CreateMesh(std::shared_ptr<Mesh> aMesh);
		void UpdateFrameBuffer();
		void UpdateObjectBuffer(ModelInstance* anInstance);

	private:



		void RenderInstances();

		HRESULT InitializeConstantBuffer(const size_t someDataSize, ComPtr<ID3D11Buffer>& aBuffer);
		HRESULT InitializeSampler(D3D11_FILTER aFilter, D3D11_TEXTURE_ADDRESS_MODE  aTileType, ComPtr<ID3D11SamplerState>& aSampler);
		HRESULT InitializeDepthBuffer(const Resolution& aResolution, ComPtr<ID3D11DepthStencilView>& aDepthBuffer);

		ComPtr<ID3D11Device> myDevice;
		ComPtr<ID3D11DeviceContext> myContext;
		ComPtr<IDXGISwapChain> mySwapChain;
		ComPtr<ID3D11RenderTargetView> myBackBuffer;

		ComPtr<ID3D11Buffer> myObjectBuffer;
		ComPtr<ID3D11Buffer> myFrameBuffer;
		ComPtr<ID3D11Buffer> myStaticBuffer;
		ComPtr<ID3D11Buffer> myGlobalLightBuffer;
		ComPtr<ID3D11DepthStencilView> myDepthBuffer;




		ComPtr<ID3D11SamplerState> mySamplerState;

		Dragonite::SparseSet<ModelInstance*> myRenderInstructions;

		//std::vector<std::shared_ptr<Mesh>> myMeshes;
		System* mySystem;
		Camera* myRenderCamera;
		RenderTarget* myRenderTarget;

		LightInfo myLightData;
		Dragonite::Windows::Resolution myViewport;

		std::shared_ptr<Texture> myCubemapTexture;
	};








}


