#pragma once
#include <wrl/client.h>
#include <vector>
#include <memory>
#include <string>

#include "Rendering/RenderObject.h"
#include "Rendering/Mesh.h"
#include "../../EngineUtilities.h"

using Microsoft::WRL::ComPtr;
using Engine::Windows::Resolution;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;

class RenderObject;
struct Shape;
enum class Primitive2D;


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
			void DrawElements();

			/*	void std::shared_ptr<RenderObject> CreateElement(Primitive aPrimitiveShape);*/
			std::shared_ptr<RenderObject> Create2DElement(Primitive2D aPrimitiveShape);
			std::shared_ptr<Mesh> CreateMesh(std::shared_ptr<Mesh> aMesh);
		private:


			Shape GetUnitTriangle();
			Shape GetUnitQuad();
			Shape GetUnitCircle();

			ComPtr<ID3D11Device> myDevice;
			ComPtr<ID3D11DeviceContext> myContext;
			ComPtr<IDXGISwapChain> mySwapChain;
			ComPtr<ID3D11RenderTargetView> myBackBuffer;


			std::vector<std::shared_ptr<RenderObject>> myRenderTargets;
			std::vector<std::shared_ptr<Mesh>> myMeshes;
			System* mySystem;
		};




	}

}


