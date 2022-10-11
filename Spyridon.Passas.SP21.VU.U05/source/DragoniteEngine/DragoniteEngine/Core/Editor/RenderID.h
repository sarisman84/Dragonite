#pragma once
#include "Core/Graphics/DirectX11/DXIncludes.h"
#include "Core/Graphics/RenderTargets/RenderTarget.h"
#include "Core/CU/Math/Vector3.h"
#include "Core/CU/Math/Vector2.h"

#include <d3d11.h> //TODO: Remove this include (forward declaration doesnt work)

#include <vector>
#include <memory>
namespace Dragonite
{
	struct CameraProfile;
	class ModelInstance;
	class Scene;
	class Mouse;
	class RenderID : public RenderTarget
	{
	public:
		RenderID();
		RenderID(GraphicalInterface* aPipeline);
		void SetTotalElementCount(const float anElementCount);
		void SetViewport(Vector2f aResolution, Vector2f anTopLeftOrigin);
		void RefreshRenderView(Vector2f aViewportResolution);
		const bool TryGetElement(Mouse* someScreenCoordPos, int& anOutputElement);
		const bool TryGetElement(const Vector2f& someScreenCoordPos, int& anOutputElement);
		void Render() override;
		bool& ViewRenderID() { return myViewRenderID; }
		void RenderIDTexture();
	private:
		
		struct RenderIDBuffer
		{
			unsigned int myID;
			Vector3f myPadding;
		};

		float myElementCount;
		Vector2f myTopLeftOrigin;
		Vector2f myCurrentResolution;


		VertexShader myRenderIDVertexShader;
		PixelShader myWriteRenderIDPixelShader;
		PixelShader myReadRenderIDPixelShader;


		DataBuffer myRenderIDBuffer;
		Scene* myScene;
		bool myViewRenderID;



	};
}


