#pragma once

#include <wtypes.h>
#include <vector>
#include <memory>
#include <algorithm>

#include <unordered_map>
#include "Pipeline/Rendering/DX/DXIncludes.h"
#include "Content/DrawData.h"
#include "entt/single_include/entt/entt.hpp"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace Dragonite
{
	struct IDrawer;
	struct IContent;

	class GraphicsEngine
	{
	public:
		~GraphicsEngine();
		void Draw(entt::registry aRegistry, void* aBackBuffer = nullptr, void* aDepthBuffer = nullptr);
		void Present();
		uint32_t SubmitShader(const char* aShader);

		template<typename Drawer>
		inline Drawer* GetDrawer() { return (Drawer*)myDrawer; }


		ID3D11RenderTargetView* BackBuffer();

	public:
		static GraphicsEngine* InitializeEngine(HWND anInstance);
	private:
		GraphicsEngine();
		void Init(HWND anInstance);
	private:
		std::unordered_map<uint32_t, std::tuple<ID3D11VertexShader*, ID3D11PixelShader*, ID3D11InputLayout*>> myShaders;

		std::vector<DrawData> myInstructions;

	private:
		IDrawer* myDrawer;
		IContent* myBackBuffer;
		IContent* myDepthBuffer;

	};



}


