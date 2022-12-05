#pragma once

#include <wtypes.h>
#include <vector>
#include <memory>
#include <algorithm>

#include <unordered_map>
#include "Pipeline/Rendering/DX/DXIncludes.h"
#include "Content/DrawData.h"
#include "entt/single_include/entt/entt.hpp"
#include "Core/CU/Math/Matrix4x4.hpp"

#include "Pipeline/Rendering/Content/Projection.h"




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
		void Draw(const Matrix4x4f& aWorldToClipMatrix, void* aBackBuffer = nullptr, void* aDepthBuffer = nullptr);
		void Present();
		void Submit(const DrawInstruct& someDataToDraw);
		void SubmitView(const Matrix4x4f& aViewMatrix);

		template<typename Drawer>
		inline Drawer* GetDrawer() { return (Drawer*)myDrawer; }


		ID3D11RenderTargetView* BackBuffer();

	public:
		static GraphicsEngine* InitializeEngine(HWND anInstance);
	private:
		GraphicsEngine();
		void Init(HWND anInstance);

		void InitContent(IContent*& someContent, const size_t aTypeSize);
		void EditBuffer(IContent*& someContent, void* someData, const size_t someDataSize);

	private:

		std::vector<DrawInstruct> myInstructions;
		
		
	private:
		ModelFactory* myModelFactory;
		MaterialFactory* myMaterialFactory;

		IDrawer* myDrawer;
		IContent* myBackBuffer;
		IContent* myDepthBuffer;

		IContent* myFrameBuffer;
		IContent* myObjectBuffer;


		Matrix4x4f myViewMatrix;
	private:
		struct FrameData 
		{
			Matrix4x4f myWorldToClipMatrix;
		};

		struct ObjectData 
		{
			Matrix4x4f myTransform;
		};

	};



}


