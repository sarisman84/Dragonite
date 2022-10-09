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

		void RefreshRenderView(Vector2f aViewportResolution);
		const bool TryGetElement(Mouse* someScreenCoordPos, int& anOutputElement);
		const bool TryGetElement(const Vector2f& someScreenCoordPos, const Vector2f& aViewport, int& anOutputElement);
		void Render() override;
	private:
		struct RenderIDBuffer
		{
			unsigned int myID;
			Vector3f myPadding;
		};
		VertexShader myRenderIDVertexShader;
		PixelShader myWriteRenderIDPixelShader;

		DataBuffer myRenderIDBuffer;
		Scene* myScene;
		bool myViewRenderID;



	};
}


