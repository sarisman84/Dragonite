#pragma once

#include <wtypes.h>
#include <vector>
#include <memory>
#include <algorithm>

#include <unordered_map>
#include "Pipeline/Rendering/DX/DXIncludes.h"
#include "Content/DrawData.h"
#include "entt/single_include/entt/entt.hpp"





namespace Dragonite
{
	struct IDrawer;
	struct IContent;

	class ModelFactory;
	class MaterialFactory;

	class GraphicsEngine
	{
	public:
		~GraphicsEngine();
		void Draw(void* aBackBuffer = nullptr, void* aDepthBuffer = nullptr);
		void Present();
		void Submit(const DrawInstruct& someDataToDraw);

		template<typename Drawer>
		inline Drawer* GetDrawer() { return (Drawer*)myDrawer; }


		ID3D11RenderTargetView* BackBuffer();

	public:
		static GraphicsEngine* InitializeEngine(HWND anInstance);
	private:
		GraphicsEngine();
		void Init(HWND anInstance);
	private:

		std::vector<DrawInstruct> myInstructions;
		
		
	private:
		ModelFactory* myModelFactory;
		MaterialFactory* myMaterialFactory;

		IDrawer* myDrawer;
		IContent* myBackBuffer;
		IContent* myDepthBuffer;

	};



}


