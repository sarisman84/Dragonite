#pragma once
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <string>
#include <queue>

#include "../../EngineUtilities.h"
#include "Utilities/Math/Matrix.h"
#include "Utilities/Math/Vector.h"

using Microsoft::WRL::ComPtr;
using Engine::Windows::Resolution;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;
struct ID3D11DepthStencilView;

class RenderObject;
struct Shape;
enum class Primitive2D;
class Camera;
struct ModelInstance;

struct FrameBufferData
{
	Math::Matrix4x4f myClipSpaceMatrix;
	float myTimeDelta;
	float myTotalTime;
	float myGarbage[2];
};

struct ObjectBufferData
{
	Math::Matrix4x4f myObjectMatrix;
};


namespace Engine
{
	class System;
	namespace Graphics
	{
		



		class GraphicsEngine
		{
		public:
			GraphicsEngine();
			~GraphicsEngine();
			bool Initialize(Engine::Windows::Resolution aResolution, HWND aWindowsHandle, System* aSystem);

			inline void AddRenderInstruction(ModelInstance* anInstance)
			{
				myRenderInstructions.push(anInstance);
			}

			inline void SetRenderCamera(Camera* aCamera) { myRenderCamera = aCamera; }
			inline Camera* GetRenderCamera() noexcept { return myRenderCamera; }

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
			ComPtr<ID3D11DepthStencilView> myDepthBuffer;

			std::queue <ModelInstance*> myRenderInstructions;
			std::vector<std::shared_ptr<RenderObject>> myRenderTargets;
			//std::vector<std::shared_ptr<Mesh>> myMeshes;
			System* mySystem;
			Camera* myRenderCamera;
		};




	}

}


