#pragma once
#include "Core/Graphics/DXIncludes.h"
#include "Core/Graphics/RenderTargets/RenderTarget.h"
#include "Core/CU/Math/Vector3.h"

#include <d3d11.h> //TODO: Remove this include (forward declaration doesnt work)

#include <vector>
namespace Dragonite
{
	class Scene;
	class RenderID : public RenderTarget
	{
	public:
		RenderID();
		~RenderID();
		RenderID(GraphicsPipeline* aPipeline);

		void SetupRenderID(Scene* aCurrentScene);
		const bool OnRender() override;

		inline bool& ViewRenderID() { return myViewRenderID; }


	private:
		struct RenderIDBuffer
		{
			float myID;
			Vector3f myPadding;
		};
		VertexShader myRenderIDVertexShader;
		PixelShader myRenderIDPixelShader;
		InputLayout myRenderIDLayout;

		DataBuffer myRenderIDBuffer;


		Scene* myScene;
		bool myViewRenderID;
	};
}


