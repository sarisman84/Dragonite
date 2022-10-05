#pragma once
#include "Core/Graphics/DXIncludes.h"
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
		~RenderID() override; 
		RenderID(GraphicsPipeline* aPipeline);

		void SetupRenderID(Scene* aCurrentScene);
		const bool OnRender() override;

		inline bool& ViewRenderID() { return myViewRenderID; }

		const bool TryGetElement(Mouse* someScreenCoordPos, int& anOutputElement);

	private:
		void DrawRenderID();


		struct RenderIDBuffer
		{
			unsigned int myID;
			Vector3f myPadding;
		};
		VertexShader myRenderIDVertexShader;
		PixelShader myWriteRenderIDPixelShader;
		PixelShader myReadRenderIDPixelShader;

		DataBuffer myRenderIDBuffer;

		CameraProfile* myOriginalProfile;
		CameraProfile* myOrthographicProfile;
		Scene* myScene;
		bool myViewRenderID;
	};
}


