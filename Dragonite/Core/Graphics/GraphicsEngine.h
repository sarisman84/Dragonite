#pragma once
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <string>
#include <queue>

#include "Rendering/RenderObject.h"
#include "../../EngineUtilities.h"
#include "Graphics/Rendering/Camera.h"

using Microsoft::WRL::ComPtr;
using Engine::Windows::Resolution;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;

class RenderObject;
struct Shape;
struct ModelInstance;
enum class Primitive2D;
struct ObjectFrameData;





namespace Engine
{
	class System;
	namespace Graphics
	{
		struct ObjectBufferData
		{
			Math::Matrix4x4f myObjectMatrix;
		};

		struct FrameBufferData
		{
			Math::Matrix4x4f myClipSpaceMatrix;
		};


		struct RenderInstruction
		{
			void(*myCallback)(ModelInstance&, class GraphicsEngine*);
			ModelInstance& myInstance;
		};


		class GraphicsEngine
		{
		public:
			GraphicsEngine();
			~GraphicsEngine();
			bool Initialize(Engine::Windows::Resolution aResolution, HWND aWindowsHandle, System* aSystem);

			inline void AddRenderInstruction(void(*aCallback)(ModelInstance&, class GraphicsEngine*), ModelInstance& anInstance)
			{
				myRenderInstructions.push(RenderInstruction{ aCallback, anInstance });
			}

			inline void SetRenderCamera(Camera& aCamera) { myRenderCamera = aCamera; }
			inline Camera& GetRenderCamera() noexcept { return myRenderCamera; }

			void DrawElements();
			inline ID3D11Device* GetDevice() { return myDevice.Get(); }
			inline ID3D11DeviceContext* GetDeviceContext() { return myContext.Get(); }

			inline ComPtr<ID3D11Buffer>& ObjectBuffer() { return myObjectBuffer; }
			inline ComPtr<ID3D11Buffer>& FrameBuffer() { return myFrameBuffer; }

			/*	void std::shared_ptr<RenderObject> CreateElement(Primitive aPrimitiveShape);*/
			std::shared_ptr<RenderObject> Create2DElement(Primitive2D aPrimitiveShape);
			//std::shared_ptr<Mesh> CreateMesh(std::shared_ptr<Mesh> aMesh);
		private:


			Shape GetUnitTriangle();
			Shape GetUnitQuad();
			Shape GetUnitCircle();

			ComPtr<ID3D11Device> myDevice;
			ComPtr<ID3D11DeviceContext> myContext;
			ComPtr<IDXGISwapChain> mySwapChain;
			ComPtr<ID3D11RenderTargetView> myBackBuffer;

			ComPtr<ID3D11Buffer> myObjectBuffer;
			ComPtr<ID3D11Buffer> myFrameBuffer;

			std::queue <RenderInstruction> myRenderInstructions;
			std::vector<std::shared_ptr<RenderObject>> myRenderTargets;
			//std::vector<std::shared_ptr<Mesh>> myMeshes;
			System* mySystem;
			Camera myRenderCamera;
		};




	}

}


